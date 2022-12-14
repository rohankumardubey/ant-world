#ifndef ANT_WORLD_PLAYER_H
#define ANT_WORLD_PLAYER_H

#include "insect.h"

namespace aw {

    class Player : public Insect {
    public:
        Player();
        ~Player();

        virtual void initialize();
        virtual void process();
        virtual void render();

        void updateEnergy(float dt);

    protected:
        virtual void grab();
        virtual void drop();
        virtual void eat();

        virtual bool enterHole();
        virtual void exitHole();

        virtual bool enterVehicle();
        virtual void exitVehicle();

        void enter();
        void exit();

        void updateMotion();
        void updateAnimation();
        void interactWithBoxes();

        void updateAnimationSmooth();
        void updateMotionSmooth();

    protected:
        ysAnimationActionBinding
            m_animBlink,
            m_animIdle,
            m_animWalk,
            m_animSidestep,
            m_faceUp,
            m_faceDown,
            m_faceLeft,
            m_faceRight;

        dbasic::RenderSkeleton *m_renderSkeleton;

        ysAnimationChannel *m_legsChannel;
        ysAnimationChannel *m_antennaChannel;
        ysAnimationChannel *m_eyelidChannel;
        ysAnimationChannel *m_bodyRotationChannel;

    protected:
        float m_energy;

        // Assets ----
    public:
        static void configureAssets(dbasic::AssetManager *am);

    protected:
        static ysAnimationAction
            *AnimBlink,
            *AnimIdle,
            *AnimWalk,
            *AnimSidestep,
            *PoseFaceUp,
            *PoseFaceDown,
            *PoseFaceRight,
            *PoseFaceLeft;

        static dbasic::SceneObjectAsset *CharacterRoot;
    };

} /* namespace aw */

#endif /* ANT_WORLD_PLAYER_H */
