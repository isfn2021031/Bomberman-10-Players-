#include <string>
#include <fstream>

#include "DxLib.h"
#include "../h/Scene_Menu.h"
#include "../h/board_preview.h"
#include "../h/configMgr_def.h"
#include "../h/configMgr_players.h"
#include "../h/configMgr_teams.h"
#include "../h/configMgr_items_panel.h"
#include "../h/configMgr_items_enemy.h"
#include "../h/dialogue_withMenu.h"
#include "../h/myfunc.h"
#include "../h/language.h"
#include "../h/playerCounter.h"

using namespace std;

graphicParam    Scene_Menu::GH_logo;

Scene_Menu::Scene_Menu(ISceneChanger* changer) : BaseScene(changer)
{
    pCL = new configLoader(FILEPATH_CONF, CONFIG_MAX);

    if (GH_logo.handle == -1)
    {
        GH_logo = LoadGraphParam("textures/logo.png");
    }
    eff_title = new effect_rotaGraph3(GH_logo, 0.005);

    configLoader sCl(FILEPATH_OTHER, sCONFIG_MAX);
    string FileName = "sound/bgm/menu/" + to_string(sCl.getConfig(sCONFIG_SoundStyle)) + ".mp3";
    musicData.LoadMusicToString(FileName.c_str());

    fstream fin(FPATH_STG_PARAMETER, ios::binary | ios::in);
    fin >> values[step_Stage] >> values[step_StageStyle];
    fin.close();

    mallocMI(0);    
    previewBoard = new board_preview(values[step_Stage], values[step_StageStyle]);

    playerCounter::GetInstance().ReSetup();
}
Scene_Menu::~Scene_Menu()
{
    if (conf_ptr != nullptr)
    {
        delete conf_ptr;
    }
    if (mHandi != nullptr)
    {
        delete mHandi;
    }
    if (mI != nullptr)
    {
        delete mI;
    }
    if (previewBoard != nullptr)
    {
        delete previewBoard;
    }
    if (pCL != nullptr)
    {
        delete pCL;
    }
    if (eff_title != nullptr)
    {
        delete eff_title;
    }
    if (dialogue_ptr != nullptr)
    {
        delete  dialogue_ptr;
    }
}

void Scene_Menu::Update()
{
    BaseScene::Update();

    static bool OldCustom = false;
    bool leaderCustom = playerCounter::GetInstance().Update();
    
    if (previewBoard != nullptr)
    {
        previewBoard->Update();
    }  
    if (eff_title != nullptr)
    {
        eff_title->Update();
    }

    if (!leaderCustom)
    {
        if (dialogue_ptr != nullptr)
        {
            int retval = dialogue_ptr->Update();
            if (retval != 0)
            {
                delete dialogue_ptr;
                dialogue_ptr = nullptr;

                switch (afterProcess)
                {
                case after_game:
                    if (retval > 0)
                    {
                        mSceneChanger->ChangeScene(eScene_Game);    // �V�[�����Q�[����ʂɕύX
                    }
                    break;

                case after_restart:
                    step = step_Mode;
                    break;

                case after_exit:
                    if (retval > 0)
                    {
                        exit(-1);
                    }
                    return;

                default:
                    break;
                }
                afterProcess = after_none;
            }
        }
        else
        {
            int oldStep = step;

            switch (nowState)
            {
            case 0:
                step += mI->Update();

                if (step != oldStep)
                {
                    if (step > oldStep)
                    {
                        int old_value = values[oldStep];
                        values[oldStep] = mI->getCursor();

                        if (step >= step_Stage)
                        {
                            bool change = (old_value != values[oldStep]);
                            if (oldStep == step_Stage && values[step_Stage] == Stage_User)
                            {
                                change = true;
                                board_withGimmick::setEditFileName(getLoadFileName({ "stg file\0*.stg;\0","Load Stage" }));
                            }
                            if (change)
                            {
                                if (previewBoard != nullptr)
                                {
                                    delete previewBoard;
                                }
                                previewBoard = new board_preview(values[step_Stage], values[step_StageStyle]);
                            }
                        }
                        if (oldStep == step_Mode)
                        {
                            if (modeSelect())
                            {
                                return;
                            }
                        }
                        if (oldStep == step_Wait)
                        {
                            if (mallocConfig(mI->getCursor()))
                            {
                                return;
                            }
                            fstream fout(FPATH_STG_PARAMETER, ios::binary | ios::out);
                            fout << values[step_Stage] << " " << values[step_StageStyle];
                            fout.close();
                        }
                    }
                    mallocMI(step);
                }
                if (step < 0)
                {
                    ExitDialogue();
                    return;
                }
                if (step >= step_MAX)
                {
                    step = step_MAX - 1;
                    afterProcess = after_game;
                    if (dialogue_ptr != nullptr)
                    {
                        delete dialogue_ptr;
                        dialogue_ptr = nullptr;
                    }
                    dialogue_ptr = new dialogue_withMenu(language::getInstance()->getString({ "Are You Ready?", "�Q�[�����͂��߂܂����H" }));
                }
                break;

            case 1:
            {
                int retval = conf_ptr->Update();

                if (retval != 0)
                {
                    if (retval > 0)
                    {
                        conf_ptr->Finalize();
                    }
                    delete conf_ptr;
                    conf_ptr = nullptr;
                    nowState = 0;
                    mallocMI(step);
                }
                break;
            }
            case 2:
                mHandi->Update();
                if (mHandi->checkFinished())
                {
                    nowState = 0;
                    delete mHandi;
                    mHandi = nullptr;
                    mallocMI(step);
                }
                break;

            default:
                break;
            }
        }
    }
    if (leaderCustom > OldCustom)
    {
        mallocMI(step);
    }
    OldCustom = leaderCustom;
}
void Scene_Menu::Draw()
{
    if (previewBoard != nullptr)
    {
        previewBoard->Draw();
    }
    switch (nowState)
    {
    case 0:
        if (mI != nullptr)
        {
            mI->Draw(WINDOW_WIDTH * 0.7, WINDOW_HEIGHT * 0.25);
            mI->DrawGuideStr(WINDOW_WIDTH * 0.20, WINDOW_HEIGHT * 0.70, WINDOW_WIDTH * 0.65, WINDOW_HEIGHT * 0.80);
        }
        if (eff_title != nullptr)
        {
            eff_title->Draw(WINDOW_WIDTH * 0.425, WINDOW_HEIGHT * 0.475);
        }
        break;

    case 1:
        if (conf_ptr != nullptr)
        {
            conf_ptr->Draw();
        }
        break;

    case 2:
        if (mHandi != nullptr)
        {
            mHandi->Draw();
        }
        break;

    default:
        break;
    }
    if (dialogue_ptr != nullptr)
    {
        dialogue_ptr->Draw();
    }
    playerCounter::GetInstance().Draw();
}

void Scene_Menu::mallocMI(int _step)
{
    if (_step >= 0 && _step < step_MAX)
    {
        if (mI != nullptr)
        {
            delete mI;
            mI = nullptr;
        }
        configLoader cl(FILEPATH_CONF, CONFIG_MAX);

        switch (_step)
        {
        case step_Mode:
            mI = new menuCursorMgr(modeSelect_MAX, true);
            mI->setCursorStr(language::getInstance()->getStringVec({ 
                {"Battle",          "�o�g���Q�[��"},
                {"Player Select",   "�v���C���[�Z���N�g"},
                {"Team Select",     "�`�[���Z���N�g"},
                {"Network",         "�l�b�g���[�N"}, 
                {"Edit Stages",     "�X�e�[�W���"}, 
                {"Option",          "�I�v�V����"},
                {"Exit",            "�Q�[���I��"}
            }));
            mI->setLabelStr(language::getInstance()->getString({ "Select Mode!", "�Q�[�����[�h�������łˁI" }));
            mI->setAccentGraph(modeSelect_TeamSelect, "textures/icon/flag.png");
            mI->setAccentGraph(modeSelect_Config, "textures/icon/conf.png");
            mI->setAccentGraph(modeSelect_Edit, "textures/icon/stg.png");
            break;

        case step_Stage:
            mI = new menuCursorMgr(Stage_MAX);
            mI->setCursorStr(language::getInstance()->getStringVec(
                { {"Normal","���Ȃ���"},{"Open Field","�͂����"},{"Potholes","���Ȃڂ�"},{"Bomb Bowling","�R���R��"},
                {"Items Grab","�A�C�e���U�N�U�N"}, {"Pipe","�h�J��"},{"Belt Zone","�x���g�R���x�A"},{"Belt Zone 2","�x���g�R���x�A�Q"}, {"User","������X�e�[�W"}}));
            mI->setLabelStr(language::getInstance()->getString({ "Select stage.", "�X�e�[�W�������łˁI" }));
            mI->setAccentGraph(Stage_User, "textures/icon/stg.png");
            mI->setAccentGraph(Stage_ItemsGrab, "textures/icon/treasureChest.png"); 
            mI->setGuideStr(language::getInstance()->getStringVec(
                {
                        {
                            "Simple stage.",
                            "���Ȃ��݂̃X�^���_�[�h�ȃX�e�[�W�B"
                        },
                        {
                            "You can run but you can't hide in this one!\n""No hard blocks to crouch behind!",
                            "�n�[�h�u���b�N���܂������Ȃ��X�e�[�W�B\n������΂��傪�Ȃ��̂�\n�܂��ɂ��イ������񂾁I"
                        },
                        {
                            "A stage folt of potholes!\nBomb blasts go right over them too,\nso be carefull.",
                            "���Ȃڂ��������ς������Ă���X�e�[�W�B\n�΂��ӂ��͂��Ȃڂ����������邩��\n��������񂾁I",
                        },
                        {
                            "Kicked bombs go in the direction of arrows,\nso kick your way to victory here!",
                            "�L�b�N�����{�����₶�邵�̂ق������ɂ��낪��X�e�[�W�B\n�₶�邵�������ăL�b�N�����������I"
                        },
                        {
                            "A stash of items is visible from the start.\nGrab'em up fast and get the win!",
                            "�A�C�e�����͂��߂���݂��Ă���X�e�[�W�B\n�����΂�͂₭�A�C�e�������߂�̂�\n���傤��ւ̂����݂����I",
                        },
                        {
                            "You can hide in them, or set bombs in them!\nThere are pipes galore in the Pipes stage!",
                            "�������ȃh�J���������ꂽ�X�e�[�W�B\n�h�J���̂Ȃ��ɂ΂���������������I",
                        },
                        {
                            "Your bombs get carried away automatically.",
                            "�x���g�R���x�A�������ꂽ�X�e�[�W�B\n�Ȃ���Ă���΂�����ɂ�������񂾁I",
                        },
                        {
                            "Let's carry bombs on the belt",
                            "�x���g�R���x�A�������ꂽ�X�e�[�W�B\n�Ȃ���Ă���΂�����ɂ�������񂾁I",
                        },
                        {
                            "Load the stage you created from a \".stg\" file.",
                            "�u�X�e�[�W���v�ō�����X�e�[�W��\n�r�s�f�t�@�C�������݂������I"
                        },
                }));
            break;

        case step_StageStyle:
            mI = new menuCursorMgr(CellStyle_MAX);
            mI->setCursorStr(language::getInstance()->getStringVec(
                { {"Classic","�N���b�V�b�N"}, {"Retrospective","���g��"}, {"Desert","���΂�"}, {"Slip-Sliding","�c���c��"},
                {"Volcano","������"}, {"Darkness","������"}, {"Hyper Feet","�����Ă�"}, {"Factory", "�������傤"}, {"PowerFreaks","�M���M���p���["}}));
            mI->setLabelStr(language::getInstance()->getString({ "Select visual style.", "�X�^�C���������łˁI" }));
            mI->setGuideStr(language::getInstance()->getStringVec(
                {
                    {
                        "No gimmicks.\nIt's easy to see who has true skills in this one.",
                        "�����ǂ���̂̃X�^�C��\n�M�~�b�N���Ȃ��̂Ŏ��͂̍����킩��₷���I�H"
                    },
                    {
                        "The blast distance is 2 from the start.",
                        "�ǂ������������X�^�C���B\n�f�t�H���g�̉Η͂��Q�}�X�ɁI",
                    },
                    {
                        "Set Mine Bombs with start.",
                        "�����̃X�^�C��\n�����_���ɐݒu�����n���{���ɋC�����āI"
                    },
                    {
                        "Battle it out on a slippery ice floor.",
                        "���������߂�̂����ł��������B\n�ړ������炷���ɂ͎~�܂�Ȃ��I�I",
                    },
                    {
                        "The blast distance is strong from the start.",
                        "�ŏ�����{�������̉Η͂������f���W�����X�ȃX�^�C���I\n������悭���Ċm���ɔ����悤"
                    },
                    {
                        "In the this style,\nThe stage is covered in darkness.",
                        "�X�e�[�W�S�̂��܂�����ɁI\n�����ǂ܂�ł͂��܂�Ȃ��悤�ɋC�����悤"
                    },
                    {
                        "You run at top speed in this stage.\nBut don't run too fast and lose focus!.",
                        "�ō��X�s�[�h�ł��������X�^�C���B\n���ӂ��đ��삷��񂾁I",
                    },
                    {
                        "Items will be revived.",
                        "���Ԍo�߂ŃA�C�e�������R��������\n������ɂ��Ȃ��悤�I",
                    },
                    {
                        "No soft blocks.\nKicks & punches can be used,\nand blasts are strong from the start.",
                        "�\�t�g�u���b�N�ƃA�C�e�����Ȃ��Ȃ�B\n�͂��߂���ŋ���Ԃ�\n�L�b�N��p���`�������邼�I",
                    },
                }));
            mI->setAccentGraph(CellStyle_Speedy,        "textures/icon/speedup.png");
            mI->setAccentGraph(CellStyle_Volcano,       "textures/icon/fireup.png");
            mI->setAccentGraph(CellStyle_SlipSliding,   "textures/icon/snowCrystal.png");
            mI->setAccentGraph(CellStyle_Factory,       "textures/icon/conf.png");
            break;

        case step_Wait:
            mI = new menuCursorMgr(5);
            mI->setLock(
                {
                    false,
                    false,
                    (cl.getConfig(CONFIG_ItemDropRate) == 0),
                    (cl.getConfig(CONFIG_Enemies) == 0),
                    (cl.getConfig(CONFIG_Handicap) == 0),
                }
            );
            mI->setCursorStr(language::getInstance()->getStringVec(
                { 
                    {"Start",           "�X�^�[�g"}, 
                    {"Settings",        "�Z�b�g�A�b�v"},
                    {"Item Panels",     "�A�C�e���p�l��"},
                    {"Enemies",         "�G�L�����N�^�["},
                    {"Handi",           "�n���f"} 
                }
            ));
            mI->setLabelStr(language::getInstance()->getString({ "Final Check.", "�����ɂ�" }));
            mI->setGuideStr(language::getInstance()->getStringVec(
                { 
                    {"Start the batte!",                                "�o�g���X�^�[�g�I"},        
                    {"Choose the rules of the battle.",                 "�ΐ탋�[���Ȃǂ̐ݒ肪�ł��邼"},
                    {"Decide which itempanels appear in the stage!",    "�o������A�C�e���p�l����ݒ�ł��邼\n���ݒ肷��ɂ́u�Z�b�g�A�b�v�v��\n�u�A�C�e���p�l���v���Ȃ��ȊO�ɂ��悤"},
                    {"Decide which enemies appear in the stage!",       "�o������G�L�����N�^�[��ݒ�ł��邼\n���ݒ肷��ɂ́u�Z�b�g�A�b�v�v��\n�u�G�L�����N�^�[�v���u����v�ɂ��悤"},
                    {"Set various handicaps.",                          "�e�v���C���[�Ƀn���f�̂����Ă����ł��邼\n���ݒ肷��ɂ́u�Z�b�g�A�b�v�v��\n�u�n���f�v���u����v�ɂ��悤"}
                }
            ));
            break;

        default:
            return;
        }
        mI->setCursor(values[_step]);
    }
}
void Scene_Menu::ExitDialogue()
{
    step = 0;
    if (dialogue_ptr != nullptr)
    {
        delete dialogue_ptr;
        dialogue_ptr = nullptr;
    }
    afterProcess = after_exit;
    dialogue_ptr = new dialogue_withMenu(language::getInstance()->getString(
        { "Can You exit?", "�\�t�g���I�����܂����H" }));
}

bool Scene_Menu::mallocConfig(int c)
{
    if (c == 0)
    {
        return false;
    }
    if (conf_ptr != nullptr)
    {
        delete conf_ptr;
    }
    step = step_Wait;
    nowState = 1;

    switch (c)
    {
    case 1:
        conf_ptr = new configMgr_def;
        break;

    case 2:
        conf_ptr = new configMgr_items_panel;
        break;

    case 3:
        conf_ptr = new configMgr_items_enemy;
        break;

    case 4:
        nowState = 2;
        mHandi = new handiSelect;

        if (dialogue_ptr != nullptr)
        {
            delete dialogue_ptr;
            dialogue_ptr = nullptr;
        }
        dialogue_ptr = new dialogue(language::getInstance()->getString(
            { "Set Ability.", "�e�v���C���[��\n�ŏ����玝���Ă���A�C�e����ݒ肵�悤\n\n�݂��{���ŕ��������Ƃ��ɂ����ʂ��K�p����邼�I" }));
        break;

    default:
        return false;
    }
    return true;
}
bool Scene_Menu::modeSelect()
{
    switch (mI->getCursor())
    {
    case modeSelect_PlayerSelect:
        step = step_Mode;
        nowState = 1;

        if (conf_ptr != nullptr)
        {
            delete conf_ptr;
        }
        conf_ptr = new configMgr_players;
        return true;

    case modeSelect_TeamSelect:
        step = step_Mode;
        nowState = 1;

        if (conf_ptr != nullptr)
        {
            delete conf_ptr;
        }
        conf_ptr = new configMgr_teams;
        return true;

    case modeSelect_Config:
        mSceneChanger->ChangeScene(eScene_Option);
        return true;

    case modeSelect_Edit:
        mSceneChanger->ChangeScene(eScene_StageMaker);
        return true;

    case modeSelect_NetworkCTR:
        mSceneChanger->ChangeScene(eScene_Network);
        return true;

    case modeSelect_Exit:
        ExitDialogue();
        return true;

    default:
        break;
    }
    return false;
}