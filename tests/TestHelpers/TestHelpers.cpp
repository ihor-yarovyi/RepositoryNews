#include "gtest/gtest.h"
#include <QDir>
#include <QApplication>
#include <memory>
#include <stdexcept>

#include "rpnews/interfaces/IRepository.h"
#include "rpnews/interfaces/IRepositoryType.h"
#include "rpnews/interfaces/IRepositoryFactory.h"
#include "rpnews/git_implementation/GitRepositoryTypeImpl.h"
#include "rpnews/git_implementation/GitRepositoryImpl.h"
#include "rpnews/git_implementation/GitRepositoryFactory.h"
#include "rpnews/helpers/rpnews_types.h"
#include "rpnews/helpers/ConfigChecker.h"
#include "rpnews/helpers/RepositoryExist.h"
#include "rpnews/helpers/DeleteRepositoryFolder.h"
#include "rpnews/helpers/ErrorMessageMaker.h"
#include "rpnews/helpers/GetNewRepositoryFactory.h"
#include "rpnews/helpers/GetTimeInterval.h"


namespace
{
    void deleteFolder()
    {
        QDir dir;
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        if (dir.exists(".configs"))
        {
            dir.cd(".configs");
            dir.removeRecursively();
        }
    }

    // Enter correct your data, before that you run these tests
    std::string url("https://name@bitbucket.org/name/repository.git"); // Your test repository
    secure_string username("username"); // Your correct username
    secure_string pass("pass"); // Your correct password
}

TEST(ConfigChecker, FolderExists)
{
    deleteFolder();
    std::unique_ptr<IRepositoryFactory> ptr(new GitRepositoryFactory);
    std::unique_ptr<IRepository> rep(ptr->createRepository(url, username, pass, false));
    auto nameOfRepository = rep->getRepositoryName();
    rep.release();
    EXPECT_THROW(ConfigChecker::checkAndGetFinalPath(nameOfRepository), RepositoryExist);
}

TEST(ConfigChecker, FolderIsNotExists)
{
    deleteFolder();
    auto path = ConfigChecker::checkAndGetFinalPath("name");
    EXPECT_TRUE(!path.empty());
}

TEST(ConfigChecker, RepositoryFolderNameValid)
{
    std::string url("https://name@bitbucket.org/name/repository.git");
    EXPECT_EQ(ConfigChecker::getRepositoryFolderName(url), "repository");
}

TEST(ConfigChecker, RepositoryFolderNameValid2)
{
    std::string url("https://name@bitbucket.org/name/repository-lib.git");
    EXPECT_EQ(ConfigChecker::getRepositoryFolderName(url), "repository-lib");
}

TEST(ConfigChecker, RepositoryFolderNameValid3)
{
    std::string url("https://name@bitbucket.org/name/repository_lib.git");
    EXPECT_EQ(ConfigChecker::getRepositoryFolderName(url), "repository_lib");
}

TEST(ConfigChecker, RepositoryFolderNameValid4)
{
    std::string url("https://name@bitbucket.org/name/repository3.git");
    EXPECT_EQ(ConfigChecker::getRepositoryFolderName(url), "repository3");
}

TEST(ConfigChecker, RepositoryFolderNameValid5)
{
    std::string url("https://name@bitbucket.org/name3_name.git");
    EXPECT_EQ(ConfigChecker::getRepositoryFolderName(url), "name3_name");
}

TEST(ConfigChecker, RepositoryFolderNameValid6)
{
    std::string url("https://name@bitbucket.org/name3-name.git");
    EXPECT_EQ(ConfigChecker::getRepositoryFolderName(url), "name3-name");
}

TEST(ConfigChecker, RepositoryFolderNameValid7)
{
    std::string url("https://name@bitbucket.org/name3-na_me.git");
    EXPECT_EQ(ConfigChecker::getRepositoryFolderName(url), "name3-na_me");
}

TEST(ConfigChecker, RepositoryFolderNameValid8)
{
    std::string url("https://name@bitbucket.org/Name3-nA_me.git");
    EXPECT_EQ(ConfigChecker::getRepositoryFolderName(url), "Name3-nA_me");
}

TEST(ConfigChecker, RepositoryFolderNameValid9)
{
    std::string url("https://name@bitbucket.org/Na.me3-nA_me.git");
    EXPECT_EQ(ConfigChecker::getRepositoryFolderName(url), "Na.me3-nA_me");
}

TEST(ConfigChecker, BadURL)
{
    std::string url("https://name@bitbucket.org/name/repository./git");
    EXPECT_TRUE(ConfigChecker::getRepositoryFolderName(url).empty());
}

TEST(ConfigChecker, RepositoryNameFromPath)
{
    std::string path("user/home/repository/.git");
    EXPECT_EQ(ConfigChecker::getRepositoryFolderNameFromPath(path), "repository");
}

TEST(ConfigChecker, BadPath)
{
    std::string path("user/home/repository/.git/folder");
    EXPECT_TRUE(ConfigChecker::getRepositoryFolderNameFromPath(path).empty());
}

TEST(ConfigChecker, BadPath2)
{
    std::string path("user/home/repository/folder");
    EXPECT_TRUE(ConfigChecker::getRepositoryFolderNameFromPath(path).empty());
}

TEST(ConfigChecker, BadPath3)
{
    std::string path("user/home/repository/./git");
    EXPECT_TRUE(ConfigChecker::getRepositoryFolderNameFromPath(path).empty());
}

TEST(DeleteRepositoryFolder, Valid)
{
    QDir dir;
    dir.mkdir(".configs");
    dir.cd(".configs");
    dir.mkdir("folder");
    helpers::deleteRepositoryFolder("folder");
    helpers::deleteRepositoryFolder(".configs");
    EXPECT_TRUE(!dir.exists("folder"));
    EXPECT_TRUE(!dir.exists(".configs"));
}

TEST(DeleteRepositoryFolder, NotConfigFolder)
{
    QDir dir;
    dir.mkdir("folder");
    EXPECT_TRUE(dir.exists("folder"));
    helpers::deleteRepositoryFolder("folder");
}

TEST(GetNewRepositoryFactory, Valid)
{
    std::unique_ptr<IRepositoryFactory> factory(helpers::getNewRepositoryFactory(0));
    EXPECT_TRUE(factory != nullptr);
}

TEST(GetNewRepositoryFactory, WrongIndex)
{
    std::unique_ptr<IRepositoryFactory> factory(helpers::getNewRepositoryFactory(-1));
    EXPECT_TRUE(factory == nullptr);
}

TEST(GetTimeInterval, Valid)
{
    EXPECT_EQ(helpers::getTimeInterval(0), std::chrono::seconds(300));
    EXPECT_EQ(helpers::getTimeInterval(1), std::chrono::seconds(1800));
    EXPECT_EQ(helpers::getTimeInterval(2), std::chrono::seconds(3600));
}

TEST(GetTimeInterval, BadIndex)
{
    int i = 1;
    QApplication a(i, nullptr);
    EXPECT_EQ(helpers::getTimeInterval(-1), std::chrono::seconds(300));
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
} 
