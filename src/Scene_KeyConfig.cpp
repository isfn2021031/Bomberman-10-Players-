#include "DxLib.h"
#include "../h/Scene_KeyConfig.h"
#include "../h/playerCounter.h"
#include "../h/language.h"

Scene_KeyConfig::Scene_KeyConfig(ISceneChanger* changer) : BaseScene(changer)
{
    playerCounter::GetInstance().ReSetup();

    // �p�b�h�̏�����Ԃ� DirectInput ����擾����
    for (int i = 0; i < GetJoypadNum(); i++)
    {
        TCHAR joypadName[MAX_PATH];
        TCHAR productName[MAX_PATH];

        if (GetJoypadName(input_key::getPadID(i), joypadName, productName) == 0)
        {
            string joypadStr = joypadName;
            padNames.insert(joypadStr);
        }
    }
    if (GetJoypadNum() <= 0)
    {
        notFound = true;
        dialogue_ptr = new dialogue(language::getInstance()->getString({ "Controllers not found.", "�R���g���[���[���ڑ�����Ă��܂���" }));
    }

    mallocMI();
}
Scene_KeyConfig::~Scene_KeyConfig()
{
    if (mI_ptr != nullptr)
    {
        delete mI_ptr;
        mI_ptr = nullptr;
    }
    if (config_ptr != nullptr)
    {
        delete config_ptr;
        config_ptr = nullptr;
    }
    if (dialogue_ptr != nullptr)
    {
        delete dialogue_ptr;
    }
}

void Scene_KeyConfig::Update()
{
    BaseScene::Update();
    
    if (dialogue_ptr != nullptr)
    {
        if (dialogue_ptr->Update())
        {
            delete dialogue_ptr;
            dialogue_ptr = nullptr;

            if (notFound)
            {
                mSceneChanger->ChangeScene(eScene_Option);
            }
        }
    }
    else
    {
        if (padSelect)
        {
            if (mI_ptr != nullptr)
            {
                int retval = mI_ptr->Update();
                if (retval > 0)
                {
                    if (config_ptr != nullptr)
                    {
                        delete config_ptr;
                        config_ptr = nullptr;
                    }
                    config_ptr = new configMgr_button(mI_ptr->getNowCursorStr());
                    
                    if (dialogue_ptr != nullptr)
                    {
                        delete dialogue_ptr;
                        dialogue_ptr = nullptr;
                    }
                    dialogue_ptr = new dialogue(language::getInstance()->getString({
                        mI_ptr->getNowCursorStr() + " ",
                        mI_ptr->getNowCursorStr() + "\n�̃{�^���z�u���J�X�^�}�C�Y���悤\n\n�L�[�{�[�h��T�������ƁA�{�^���̔ԍ����m�F�ł��邼�I"
                    }));
                    padSelect = false;
                }
                else if (retval < 0)
                {
                    mSceneChanger->ChangeScene(eScene_Option);
                }
            }
        }
        else
        {
            if (config_ptr != nullptr)
            {
                int retval = config_ptr->Update();
                if (retval != 0)
                {
                    if (config_ptr != nullptr)
                    {
                        delete config_ptr;
                        config_ptr = nullptr;
                    }
                    padSelect = true;
                    mallocMI();
                }
            }
        }
    }
}
void Scene_KeyConfig::Draw()
{
    BaseScene::Draw();      // �e�N���X�̕`�惁�\�b�h���Ă�

    if (padSelect)
    {
        if (mI_ptr != nullptr)
        {
            mI_ptr->Draw(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
        }
    }
    else
    {
        if (config_ptr != nullptr)
        {
            config_ptr->Draw();
        }
    }
    if (dialogue_ptr != nullptr)
    {
        dialogue_ptr->Draw();
    }
}
void Scene_KeyConfig::mallocMI()
{
    // �Z�b�g����x�N�^�[�ɕϊ�
    vector<string> vec_padNames(padNames.begin(), padNames.end());

    if (mI_ptr != nullptr)
    {
        delete mI_ptr;
        mI_ptr = nullptr;
    }
    // �Q�[���p�b�h��I�ԃ��j���[�쐬
    mI_ptr = new menuCursorMgr(vec_padNames.size());
    mI_ptr->setCursorStr(vec_padNames);
}