#include <QStyle>
#include <QDesktopWidget>
#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QDir>
#include <stdexcept>
#include "AddNewRepository.h"
#include "ui_AddNewRepository.h"
#include "rpnews/git_implementation/GitRepositoryFactory.h"
#include "rpnews/helpers/ConfigChecker.h"
#include "rpnews/helpers/SaveConfig.h"


AddNewRepository::AddNewRepository(QWidget *parent) :
    QDialog(parent),
    m_UI(new Ui::AddNewRepository)
{
    m_UI->setupUi(this);
    this->setGeometry(
                QStyle::alignedRect(
                    Qt::LeftToRight,
                    Qt::AlignCenter,
                    this->size(),
                    qApp->desktop()->availableGeometry()
                    )
                );
    initializeComboBoxRepositoryType();
    on_ComboBox_RepositoryType_activated(0);
}

AddNewRepository::~AddNewRepository()
{
    delete m_UI;
}

void AddNewRepository::closeEvent(QCloseEvent* event)
{
    if (this->isVisible())
    {
        event->ignore();
        this->hide();
    }
}

void AddNewRepository::showEvent(QShowEvent* event)
{
    m_RepositoryFactory.reset();
    m_Repository.reset();
    m_UI->Button_Connect->setEnabled(true);
    m_UI->Button_Add_Save->setEnabled(false);
    m_UI->Edit_URL->clear();
    m_UI->Edit_Login->clear();
    m_UI->Edit_Password->clear();
    m_UI->ComboBox_BranchName->clear();
    m_UI->ComboBox_BranchName->setEnabled(false);
    m_UI->ComboBox_TimeInterval->clear();
    m_UI->ComboBox_TimeInterval->setEnabled(false);
    initializeComboBoxRepositoryType();
    on_ComboBox_RepositoryType_activated(0);
    m_RepositoryIsReady = false;
    event->accept();
}

void AddNewRepository::initializeComboBoxRepositoryType()
{
    m_UI->ComboBox_RepositoryType->addItem("Git");
}

void AddNewRepository::on_ComboBox_RepositoryType_activated(int index)
{
    switch (index)
    {
        case 0:
        {
            m_RepositoryFactory = std::make_unique<GitRepositoryFactory>();
            break;
        }
        default:
        {
            break;
        }
    }
}

void AddNewRepository::on_Button_Connect_clicked()
{
    auto url = m_UI->Edit_URL->text().toStdString();
    auto login = m_UI->Edit_Login->text().toStdString();
    auto pass = m_UI->Edit_Password->text().toStdString();
    std::string folderName;

    if (url.empty() || login.empty() || pass.empty())
    {
        QMessageBox::warning(this, "Warning", "Input your credentials!", QMessageBox::Ok);
    }
    else
    {
        folderName = ConfigChecker::getRepositoryFolderName(url);
        try
        {
            m_Repository.reset(m_RepositoryFactory->createRepository(url, login.c_str(), pass.c_str(), false));
            m_Repository->prepareRepository();
            m_Repository->prepareBranches();
            initializeComboBoxBranchName();
            on_ComboBox_BranchName_activated(0);
            initializeComboBoxTimeInterval();
            on_ComboBox_TimeInterval_activated(0);
            m_UI->Button_Add_Save->setEnabled(true);
            m_UI->Button_Connect->setEnabled(false);
        }
        catch (const std::logic_error& e)
        {
            deleteRepositoryFolder(folderName);
            QMessageBox::critical(this, "Error", e.what(), QMessageBox::Critical, QMessageBox::Ok);
        }
    }
}

void AddNewRepository::deleteRepositoryFolder(const std::string& name)
{
    QDir dir;
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);

    if (dir.current().exists(".configs"))
    {
        dir.cd(".configs");
        dir.cd(name.c_str());
        dir.removeRecursively();
    }
}

void AddNewRepository::initializeComboBoxBranchName()
{
    auto nameOfBranches = m_Repository->getBranchName();
    for (const auto& i : nameOfBranches)
    {
        m_UI->ComboBox_BranchName->addItem(i.c_str());
    }
    m_UI->ComboBox_BranchName->setEnabled(true);
}

void AddNewRepository::initializeComboBoxTimeInterval()
{
    m_UI->ComboBox_TimeInterval->addItem("5 minutes");
    m_UI->ComboBox_TimeInterval->addItem("30 minutes");
    m_UI->ComboBox_TimeInterval->addItem("1 hour");
    m_UI->ComboBox_TimeInterval->setEnabled(true);
}

void AddNewRepository::on_ComboBox_BranchName_activated(int index)
{
    m_Repository->setCurrentBranch(static_cast<size_t>(index));
}

void AddNewRepository::on_ComboBox_TimeInterval_activated(int index)
{
    switch (index)
    {
        case 0:
        {
            std::chrono::minutes min {5};
            m_TimeForSynchronization = min;
            break;
        }
        case 1:
        {
            std::chrono::minutes min {30};
            m_TimeForSynchronization = min;
            break;
        }
        case 2:
        {
            std::chrono::hours hours {1};
            m_TimeForSynchronization = hours;
            break;
        }
    }
}

void AddNewRepository::on_Button_Add_Save_clicked()
{
    m_Repository->saveConfig();
    SaveConfig::saveGUIConfig(".configs/" + m_Repository->getRepositoryName() + "/", m_Repository->getCurrentBranchName(), m_Repository->getCurrentBranchIndex(), m_TimeForSynchronization);
    m_RepositoryIsReady = true;
    this->close();
}

void AddNewRepository::on_Button_Cancel_clicked()
{
    this->close();
}

bool AddNewRepository::repositoryIsReady() const
{
    return m_RepositoryIsReady;
}

std::unique_ptr<IRepository> AddNewRepository::getRepository()
{
    return std::move(m_Repository);
}

std::chrono::seconds AddNewRepository::getIntervalTime() const
{
    return m_TimeForSynchronization;
}
