void AHealthPickup::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AHealthPickup::OnBeginOverlap);
}