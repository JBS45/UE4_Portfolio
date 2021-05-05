#pragma once

#include "Test.h"
#include "BaseEnum.generated.h"

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
	E_BATTLE UMETA(DisplayName = "Battle"),
	E_HIT UMETA(DisplayName = "Hit"),
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
enum class EMonsterState : uint8 {
	E_CREATE = 0 UMETA(DisplayName = "NONE"),
	E_IDLE = 1 UMETA(DisplayName = "IDLE"),
	E_BATTLE= 2 UMETA(DisplayName = "BATTLE"),
	E_DEAD = 3 UMETA(DisplayName = "DEAD"),
};

UENUM(BlueprintType)
enum class EMonsterPartsType : uint8 {
	E_HEAD = 0 UMETA(DisplayName = "Head"),
	E_BODY = 1 UMETA(DisplayName = "Body"),
	E_LEFTHAND = 2 UMETA(DisplayName = "LeftHand"),
	E_RIGHTHAND = 3 UMETA(DisplayName = "RightHand"),
	E_WING = 4 UMETA(DisplayName = "Wing"),
	E_LEFTLEG = 5 UMETA(DisplayName = "LeftLeg"),
	E_RIGHTLEG = 6 UMETA(DisplayName = "RightLeg"),
	E_TAIL = 7 UMETA(DisplayName = "Tail"),
	E_ALLBODY = 8 UMETA(DisplayName = "ALL")
};

UENUM(BlueprintType)
enum class EDamageType : uint8 {
	E_NORMAL = 0 UMETA(DisplayName = "Normal"),
	E_KNOCKBACK = 1 UMETA(DisplayName = "KnockBack"),
	E_KNOCKDOWN = 2 UMETA(DisplayName = "KnockDown"),
	E_STUN=  3 UMETA(DisplayName = "Stun"),
	E_ROAR = 4 UMETA(DisplayName = "Roar"),
};
UENUM(Meta = (Bitflags))
enum class EMonsterBrokenParts {
	E_NONE = 0x01 << 0,
	E_HEAD = 0x01 << 1,
	E_BODY = 0x01 << 2,
	E_LEFTHAND = 0x01 << 3,
	E_RIGHTHAND = 0x01 << 4,
	E_WING = 0x01 << 5,
	E_LEFTLEG = 0x01 << 6,
	E_RIGHTLEG = 0x01 << 7,
	E_TAIL = 0x01 << 8,
};
ENUM_CLASS_FLAGS(EMonsterBrokenParts);


UENUM(Meta = (Bitflags))
enum class ECommnadPriority {
	E_LOW = 0 UMETA(DisplayName = "Low"),
	E_NORMAL = 1 UMETA(DisplayName = "Normal"),
	E_HIGH= 2 UMETA(DisplayName = "High"),
};

UENUM(BlueprintType)
enum class EMoveKey : uint8 {
	E_FORWARD =0 UMETA(DisplayName = "W"),
	E_BACKWARD = 1 UMETA(DisplayName = "S"),
	E_RIGHT =2 UMETA(DisplayName = "D"),
	E_LEFT = 3 UMETA(DisplayName = "A"),
	E_ALLMOVE = 4 UMETA(DisplayName = "AnyMoveKey"),

};

UENUM(BlueprintType)
enum class EActionKey : uint8 {

	E_EVADE =0 UMETA(DisplayName = "SPACEBAR"),
	E_LEFTCLICK =1  UMETA(DisplayName = "LMB"),
	E_RIGHTCLICK = 2 UMETA(DisplayName = "RMB"),
	E_THUMBCLICK = 3 UMETA(DisplayName = "THUMBMB"),
};


DECLARE_DELEGATE(FVoidDelegate);
DECLARE_DELEGATE_OneParam(FVoidDelegateOne, ECharacterState);
DECLARE_DELEGATE_OneParam(FVoidDelegateBool, bool);
DECLARE_DELEGATE_OneParam(FVoidDelegateAnimMontage, UAnimMontage*);
DECLARE_DELEGATE_FourParams(FPlayerAttackDel, class ABasePlayerController*, FVector, int32, bool);


DECLARE_MULTICAST_DELEGATE(FVoidMultiDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterState, ECharacterState);
DECLARE_MULTICAST_DELEGATE_OneParam(FWeaponType, EWeaponType);
DECLARE_MULTICAST_DELEGATE_OneParam(FCharacterSprint, bool);
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);