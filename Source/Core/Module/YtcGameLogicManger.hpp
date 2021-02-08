#ifndef YTC_GAME_LOGIC_MANAGER_HPP
#define YTC_GAME_LOGIC_MANAGER_HPP

#include "../Utility/YtcSingleton.hpp"

namespace YtcGE
{
    class GameLogicManager : public Singleton<GameLogicManager>
    {
        MARK_AS_SINGLETON();
    public:
        void Update();
    };
}

#endif