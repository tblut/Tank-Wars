#pragma once

#include <vector>
#include <iostream>
#include <algorithm>
#include <memory>

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletCollision\CollisionShapes\btBoxShape.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Wavefront.h"
#include "Mesh.h"
#include "MeshInstance.h"
#include "MeshTools.h" // Won't be needed in the final version
#include "Renderer.h"

namespace tankwars {
	class Tank {
	public:
		struct Bullet {
			Bullet( btRigidBody* body, MeshInstance inst)
                : bulletBody(body), bulletMeshInstance(inst)
            {
            }
            
            bool active = true;
			bool disableMe = false;
			btRigidBody* bulletBody;
			MeshInstance bulletMeshInstance;
		};

		Tank(btDiscreteDynamicsWorld *dynamicsWorld, Renderer& renderer, btVector3 startingPosition);

		void addWheels();
		void update();
		void turn(bool left);
		void drive(bool forward);
		void driveBack(bool backward);
		glm::vec3 getDirectionVector();
		glm::vec3 offset();
		btRaycastVehicle* getAction();
		MeshInstance* getTankMeshInstance(int i);
		glm::vec3 getPosition();
		btRigidBody* getRigidBody();

		//Controller Functions
		void breakController();
		void driveController(float val);
		void turnController(float val);
		void turnTurretController(float val);
		void turnHeadAndTurretController(float val);
		void shoot();
		void adjustPower(bool increase);

	private:
        void initializeTankMeshInstances(btVector3 startPos);
		void setTankTuning();

		Renderer* renderer;
        btDiscreteDynamicsWorld* dynamicsWorld;
		std::unique_ptr<btCollisionShape> tankBoxShape;
        std::unique_ptr<btCompoundShape> compoundShape;
		std::unique_ptr<btMotionState> tankMotionState;
        std::unique_ptr<btRigidBody> tankChassis;
        std::unique_ptr<btDefaultVehicleRaycaster> tankVehicleRaycaster;
        std::unique_ptr<btRaycastVehicle> tank;

		btRaycastVehicle::btVehicleTuning tankTuning;
        

		//Tank Meshes and MeshInstances
		glm::mat4x4 tankModelMat;
		Material tankMaterial;
        std::unique_ptr<Mesh> tankBodyMesh;
        std::unique_ptr<Mesh> tankHeadMesh;
        std::unique_ptr<Mesh> tankCanonMesh;
        std::unique_ptr<Mesh> tankLeftFrontWheelMesh;
        std::unique_ptr<Mesh> tankRightFrontWheelMesh;
        std::unique_ptr<Mesh> tankLeftBackWheelMesh;
        std::unique_ptr<Mesh> tankRightBackWheelMesh;

		// 0 = tankBody / 1 = tankHead / 2 = turret / 3-6 = wheels
		std::vector<MeshInstance> tankMeshInstances;					// how to add this shit to the renderer?
		
		//End Tank Meshes and MeshInstances

		//Tank Physics Variables

		float maxEngineForce = 3000.f;
		float defaultBreakingForce = 10.f;
		float maxBreakingForce = 100.f;					// what is this for?0.o
		float tankEngineForce = 0.f;
		float tankBreakingForce = 0.f;// defaultBreakingForce;

		float	steeringIncrement = 0.04f;
		float	steeringClamp = 0.2f;
		float	tankSteering = 0.f;

		btScalar mass = 700;
		btScalar wheelWidth = 0.4f;
		btScalar frontWheelRadius = .5f;//0.838f;
		btScalar backWheelRadius = .5f; //1.f;
		btScalar suspensionRestLength = 0.6f;
		btVector3 wheelDirection;
		btVector3 wheelAxle;

		//End Tank Physics Variables

		//Tank Movement Variables
		btScalar shootingPower = 20;				//not adjusted
		btScalar shootingPowerIncrease = 10;		//not adjusted
		btScalar shootingPowerMax = 100;			//not adjusted

		btScalar turretMinAngle = -.06f;
		btScalar turretMaxAngle = .5f;				//not adjusted
		btScalar turretRotationAlpha = .01f;		//not adjusted
		btScalar turretAngle = .0f;
		btScalar headAndTurretRotationAlpha = .01f;	//not adjusted
		btScalar headAndTurretAngle = .0f;

		//End Tank Movement Variables
		class BulletHandler {
		public:
			BulletHandler(btDynamicsWorld* dynamicsWorld, Renderer& renderer);
			void createNewBullet(btTransform& tr, btScalar headAngle, btScalar turretAngle, btScalar power);
			void updateBullets();
			void removeBullet(Bullet& bul);
		private:
			int tankID;
			btDynamicsWorld* dynamicsWorld;
			Renderer& renderer;
			btSphereShape bulletShape;
			Mesh bulletMesh;
			size_t bulletMax = 10;
			//Bullet* bullets[5000];					//replace with an std::vector
			std::vector<Bullet> bullets;
			Material bulletMat;
		};

		BulletHandler bulletHandler;
	};
}
