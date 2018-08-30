#include "RepositoryWrapper.h"
#include <stdexcept>

git_wrapper::RepositoryWrapper::RepositoryWrapper(const std::string& path)
{
    check(path);
}

git_wrapper::RepositoryWrapper::~RepositoryWrapper()
{
    if (m_Repo != nullptr)
    {
        git_repository_free(m_Repo);
    }
}

void git_wrapper::RepositoryWrapper::check(const std::string& path)
{
    if (git_repository_init(&m_Repo, path.data(), false) != 0)
    {
        const git_error *lastError = giterr_last();
        std::string error("problem with git_repository_init, error message : '");
        error += +lastError->message;
        error += "'";
        throw std::logic_error(error);
    }
}

bool git_wrapper::RepositoryWrapper::create(const std::string& path)
{
    if (m_Repo == nullptr)
    {
        check(path);
        return true;
    }
    return false;
}

git_repository* git_wrapper::RepositoryWrapper::getPointer() const
{
    return m_Repo;
}