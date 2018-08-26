#include "GitRemoteWrapper.h"
#include <stdexcept>

git_wrapper::GitRemoteWrapper::GitRemoteWrapper(git_wrapper::RepositoryWrapper &repo, const std::string& url)
{
    if (git_remote_create(&m_Remote, repo.getPointer(), "origin", url.data()) != 0)
    {
        const git_error* lastError = giterr_last();
        std::string error("problem with git_remote_create, error message : '");
        error += lastError->message;
        error += "'";
        throw std::logic_error(error);
    }
}

git_wrapper::GitRemoteWrapper::~GitRemoteWrapper()
{
    if (m_Remote != nullptr)
    {
        git_remote_free(m_Remote);
    }
}

git_remote* git_wrapper::GitRemoteWrapper::getPointer() const
{
    return m_Remote;
}