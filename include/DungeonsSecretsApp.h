#ifndef CGAT3_DUNGEONS_SECRETS_APP_H_
#define CGAT3_DUNGEONS_SECRETS_APP_H_

#include "AppStateManager.hpp"

namespace APP
{
    class DungeonsSecretsApp
    {
    public:
        DungeonsSecretsApp();
        ~DungeonsSecretsApp();

        void start();
        
    private:
        AppStateManager *m_pAppStateManager;
    }; //end of class DungeonsSecretsApp.

} //end of namespace APP.

#endif
