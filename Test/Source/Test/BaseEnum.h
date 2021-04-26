#pragma once

#include "Test.h"
#include "BaseEnum.generated.h"


DECLARE_DELEGATE(FVoidDelegate);
DECLARE_DELEGATE_OneParam(FVoidDelegateOne, ECharacterState);
DECLARE_DELEGATE_OneParam(FVoidDelegateBool, bool);

DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UENUM(BlueprintType)
enum class EInputKey : uint8 {
	E_NONE UMETA(DisplayName = "NONE"),
	E_FORWARD UMETA(DisplayName = "FORWARD"),
	E_BACKWARD UMETA(DisplayName = "BACKWARD"),
	E_RIGHT UMETA(DisplayName = "RIGHT"),
	E_LEFT UMETA(DisplayName = "LEFT"),
	E_EVADE UMETA(DisplayName = "EVADE"),
	E_LEFTCLICK UMETA(DisplayName = "LEFTCLICK"),
	E_RIGHTCLICK UMETA(DisplayName = "RIGHTCLICK"),
	E_LEFTANDRIGHTCLICK UMETA(DisplayName = "LEFTANDRIGHTCLICK"),

};
UENUM(BlueprintType)
enum class EControlState : uint8 {
	E_NOINPUT = 0 UMETA(DisplayName = "NOINPUT"),
	E_OPTION UMETA(DisplayName = "OPTION"),
	E_IDLE UMETA(DisplayName = "IDLE"),
	E_BATTLE UMETA(DisplayName = "BATTLE"),
};
UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	E_NONE UMETA(DisplayName = "None"),
	E_IDLE UMETA(DisplayName = "Idle"),
	E_SPRINT UMETA(DisplayName = "Sprint"),
	E_EXHAUST UMETA(DisplayName = "Exhuast"),
	E_BATTLE UMETA(DisplayName = "Battle"),
	E_ATTACKING UMETA(DisplayName = "Attack"),
	E_CHAIN UMETA(DisplayName = "ChainCombo"),
	E_EVADE UMETA(DisplayName = "Evade"),
	E_HIT UMETA(DisplayName = "Hit"),
	E_KNOCKBACK UMETA(DisplayName = "Knockback"),
	E_DOWN UMETA(DisplayName = "Down"),
	E_DEAD UMETA(DisplayName = "Dead"),
};
UENUM(BlueprintType)
enum class EMovementState : uint8 {
	E_IDLE UMETA(DisplayName = "Idle"),
	E_SPRINT UMETA(DisplayName = "Sprint"),
	E_EXHAUST UMETA(DisplayName = "Exhuast"),
};
UENUM(BlueprintType)
enum class EEquipmentState : uint8 {
	E_WEAPON = 0 UMETA(DisplayName = "Weapon"),
	E_HEAD UMETA(DisplayName = "Head"),
	E_CHEST UMETA(DisplayName = "Chest"),
	E_LEG UMETA(DisplayName = "Leg"),
};
UENUM(BlueprintType)
enum class EWeaponType : uint8 {
	E_NOWEAPON = 0 UMETA(DisplayName = "NOWEAPON"),
	E_DUAL = 1 UMETA(DisplayName = "DUAL"),
	E_HAMMER = 3 UMETA(DisplayName = "Hammer"),
};
UENUM(BlueprintType)
enum class ECollisonType : uint8 {
	E_HUMAN = 0 UMETA(DisplayName = "Human"),
	E_DRAGON = 1 UMETA(DisplayName = "Dragon"),
	E_WOLF = 2 UMETA(DisplayName = "Wolf"),
};
UENUM(BlueprintType)
enum class EDetectCollisionType : uint8 {
	E_NONE = 0 UMETA(DisplayName = "none"),
	E_PLAYER =1 UMETA(DisplayName = "player"),
	E_MONSTER = 2 UMETA(DisplayName = "monster"),
};
UENUM(BlueprintType)
enum class EMonsterStateType : uint8 {
	E_NONE = 0 UMETA(DisplayName = "none"),
	E_IDLE = 1 UMETA(DisplayName = "idle"),
	E_FIND = 2 UMETA(DisplayName = "find"),
	E_BATTLE = 3 UMETA(DisplayName = "Battle"),
	E_HIT = 4 UMETA(DisplayName = "Hit"),
	E_DEAD = 5 UMETA(DisplayName = "Dead"),
};