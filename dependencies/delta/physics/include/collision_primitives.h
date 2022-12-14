#ifndef DELTA_BASIC_COLLISION_PRIMITIVES_H
#define DELTA_BASIC_COLLISION_PRIMITIVES_H

#include "delta_core.h"

namespace dphysics {

    struct BoxPrimitive {
        ysQuaternion Orientation;
        ysVector Position;
        float HalfWidth;
        float HalfHeight;

        void GetBounds(ysVector &minPoint, ysVector &maxPoint) const;
    };

    struct CirclePrimitive {
        ysVector Position;
        float Radius;

        void GetBounds(ysVector &minPoint, ysVector &maxPoint) const;
    };

    struct RayPrimitive {
        ysVector Position;
        ysVector Direction;
        float MaxDistance;

        ysVector RelativeDirection;
    };

    class RigidBody;
    class CollisionObject;
    class RigidBodySystem;

    class Collision : public ysObject {
        friend RigidBodySystem;

    public:
        Collision();
        Collision(Collision &collision);
        ~Collision();

        float m_penetration;
        ysVector m_normal;
        ysVector m_position;

        union {
            struct {
                RigidBody *m_body1;
                RigidBody *m_body2;
            };

            RigidBody *m_bodies[2];
        };

        union {
            struct {
                CollisionObject *m_collisionObject1;
                CollisionObject *m_collisionObject2;
            };

            CollisionObject *m_collisionObjects[2];
        };

        bool m_sensor;
        float m_restitution;
        float m_staticFriction;
        float m_dynamicFriction;

        void UpdateInternals(float timestep);

        Collision &operator=(Collision &collision);

        bool IsGhost() const;

    protected:
        ysVector m_relativePosition[2];
        ysMatrix m_contactSpace;
        ysVector m_contactVelocity;

        float m_desiredDeltaVelocity;


        void CalculateDesiredDeltaVelocity(float timestep);

        ysVector CalculateLocalVelocity(int bodyIndex, float timestep);

    private:
        void CalculateContactSpace();
    };

} /* namespace dbasic */

#endif /* DELTA_BASIC_COLLISION_PRIMITIVES_H */
