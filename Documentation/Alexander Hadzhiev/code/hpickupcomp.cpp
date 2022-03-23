HealthPickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SM_HealthPickup"));
HealthPickupMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

RootComponent = HealthPickupMesh;

BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
BoxCollision->SetBoxExtent(FVector(75.0f, 60.0f, 60.0f));
BoxCollision->SetupAttachment(RootComponent);
BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);