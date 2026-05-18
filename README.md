# Don
 언리얼 게임 클라이언트 포트폴리오 - 'Don'

## 1인 프로젝트

## 개발 환경 : Unreal Engine 5.2, Git, Rider

<details>
 <summary>3D 뱀파이어 서바이벌</summary>

 <br><img width="409" height="429" alt="Image" src="https://github.com/user-attachments/assets/7191d90b-b254-4b12-9a9a-7bd03c5b7981" /><br>

 
## [포트폴리오 영상](https://youtu.be/gn_eyMzty2I)

## 게임 장르

3D 뱀파이어 서바이벌류


## 기획 배경

3D 언리얼 엔진이라는 **무거운 기능 기반의 환경**에서 수많은 AI 오브젝트를 **효율적으로 관리하고 최적화**하는 방법에 대한 기술적 호기심이 생겼고, 성능 분석 및 최적화라는 **기술적 목표**를 추구하는 과제로 발전하게 되었습니다.

 
## 목차
* [최적화 시도들](#최적화-시도)
* [트러블 슈팅](#트러블-슈팅)
* [인벤토리](#인벤토리)
* [보스 AI](#보스-ai)
* [아이템](#아이템-구조)
* GAS

## 최적화 시도

<details>
 <summary>최적화 시도</summary>

 ## 문제 상황
 적이 **50마리 정도가 스폰됐을 때, 프레임이 73 FPS에서 39 FPS로 급락**하는 심각한 성능 저하가 발생<br>
 
 **<문제 원인 파악>** <br>
 Stat Game을 통한 프로파일링 <br>
 
 <img width="845" height="145" alt="Image" src="https://github.com/user-attachments/assets/8668a706-bfd7-423a-bf53-d373e305186b" />
 
 ⇒ **Char Movement Total, EndScopedMovementUpdate Time, UpdateOverlaps Time, PerformOverlapQuery Time** 항목에서 많은 비용이 발생
 
 <br>
 
 **<시도한 해결 방안>** <br>
 1. Enemy Object **전용 콜리전** 사용 및 **RVO** 사용
 2. Overlap 관련 비용 원인 탐색 → **Mesh overlap events**이 불필요하게 켜져 있었고, 이를 해제
 3. **Character Movement Component → Floating Pawn Movement**로 변경(네트워크, 물리 상호작용 비용 최소화)
 4. 불필요한 물리 충돌 **Ignore** 설정 및 **AI 뭉침 현상**을 완화하기 위해 각 AI에게 플레이어 주변의 랜덤 좌표를 할당하는 <br>
    커스텀 [**BTTaskNode_MoveToLocationAndRepath**](Source/Don/Private/AI/BTTask/BTTask_MoveToLocationAndRepath.cpp) 노드 구현
 ⇒ **UpdateOverlaps Time, PerformOverlapQuery Time** 감소 <br>
 
 **<중간 결과>** <br>
 프레임 드랍의 주요 원인 4가지의 비용을 현저히 낮춤 <br> 
 **39** FPS -> **60** FPS <br>
 **25** ms -> **17** ms <br>
 (오브젝트 **50개** 기준)<br><br>
 
 <img width="848" height="203" alt="Image" src="https://github.com/user-attachments/assets/1d58f37b-d417-4c1d-86ab-da501f262399" />
 
 
 
 **<추가 실험>**
 
 오브젝트를 **100개로** 늘렸을 때 : **33 FPS** <br>
 렌더링 **Lumen, 안티 앨리어싱** 껐을 때 : **46 FPS** <br>
 추가로 병목이 발생한 지점을 파악하기 위해 **stat unit**을 활용하여 프레임이 걸린 시간과 각 요소들을 파악
 
 <img width="141" height="166" alt="Image" src="https://github.com/user-attachments/assets/1ca24787-28d8-48ed-a8a1-eacd2b00b8cc" />
 
 1. Frame : 21.52ms
 2. Game : 13.66ms
 3. Draw : 7.01ms
 4. RHIT : 6.54ms
 5. **GPU Time : 21.11ms**
 
 * Game Thread -> Render Thread로 게임 로직 처리 데이터 전달
 * Render Thread -> Render Hardware Interface로 생성한 드로우 콜 전달
 * RHI Thread -> GPU가 이해하는 저수준 언어로 파싱 및 전달
 * **GPU : 드로우 콜 처리 => 이 부분에서 21.11ms가 발생** <br>
 
 기본적으로 Game Thread가 1프레임 정도는 앞서서 처리할 수 있지만 최종적으로 GPU에서 처리하는 시간이 1프레임(설정 값 기준)을 초과한 지연이 발생. <br>
 GPU를 제외한 세 개의 스레드에서 GPU 작업 완료 후에 처리될 동기화를 기다리기 때문에 프레임 드랍 초래. <br>
 이를 실제로 파악하기 위해 **Unreal Insights** 툴을 사용
 
 <img width="1463" height="558" alt="Image" src="https://github.com/user-attachments/assets/5943326b-5b6e-4dd6-9180-dc6f00c690b8" />
 
 <img width="360" height="32" alt="Image" src="https://github.com/user-attachments/assets/19621ddf-6eff-40d1-88b5-579def6690cb" />
 
 <img width="360" height="32" alt="Image" src="https://github.com/user-attachments/assets/e47bc3cd-9c5f-4e4b-a6e8-f6959ef68e66" />
 
 <img width="360" height="32" alt="Image" src="https://github.com/user-attachments/assets/3f1ba601-4330-4e8b-a5b5-cdd792c8893b" />
 
 <img width="360" height="32" alt="Image" src="https://github.com/user-attachments/assets/18c5e5de-d14b-46ab-a0e6-08e667aff802" />
 
 
 
 * Game Thread -> GameThreadWaitForTask **8.7ms**
 * Render Thread -> WaitUntilTasksComplete **13.3ms**
 * RHI Thread -> WaitUntilTasksComplete **6.8ms**, WaitForTasks **7.2ms 이상**
 
 <img width="718" height="16" alt="Image" src="https://github.com/user-attachments/assets/abbfa99a-0583-4c16-9c51-5452c2f1db05" />
 
 **<GPU 처리 - 언리얼 렌더링 패스>**
 * Prepass **2.3ms**
 * Basepass **6.3ms**
 * RenderAnisotropyPass **3ms**
 * ShadowDepths **5.2ms**
 
 => 주로 Mesh의 **Triangles 수에** 비례하는 처리 비용들 <br>
 * 가벼운 스켈레탈 메시로 교체 후 LOD의 **Cull distance** 설정
 * 애니메이션 적용시키고 멀리 있는 애니메이션 업데이트 주기 감소(**Enable Update Rate Optimizations**) 활성화
   
 <img width="1920" height="1080" alt="Image" src="https://github.com/user-attachments/assets/0023eaaa-4803-4729-9b09-738c933cb750" />
 
 <img width="170" height="181" alt="Image" src="https://github.com/user-attachments/assets/0dbcae01-9375-4c25-b8c3-8349ee71df97" />
 
 
 **<최종 결과>**
 * 2배 가까이 줄어든 드로우 콜 및 Frame 소요 시간
 * 200개 가까이 소환해도 60 FPS 유지
 </details>

 
 ## 트러블 슈팅

  <details>
   <summary>스킬 이펙트로 인한 프레임 스파이크</summary>
   <br>
   
   스킬 피격 이펙트가 오브젝트마다 발생하기 때문에, 뭉쳐 있는 경우 프레임이 급격하게 떨어짐

   <br>
   <img width="568" height="591" alt="Image" src="https://github.com/user-attachments/assets/99d69c10-6aa3-4f51-b245-d9d58f7ee9c3" />
   <br>
   <br>
   
   => 시간과 거리 임계값을 추가해서 두 조건을 모두 만족한 경우에만 이펙트를 발생시킴. 순간적으로 튀는 상황 방지

   <br>
   <img width="1031" height="572" alt="Image" src="https://github.com/user-attachments/assets/a4c8bf68-a576-4768-9c97-35ab90e00c87" />

   <br>
   <img width="537" height="563" alt="Image" src="https://github.com/user-attachments/assets/b83e16b8-744f-4d58-ae0c-15ee1ae757cf" />
   <br>

   => 스파이크 현상 방지
  </details>

  <details>
   <summary>Physics 적용 시, Collision의 위치 고정 문제</summary>
   <br>
   넉백 공격을 통해 Skeltal Mesh의 Physics를 활성화시키면 자연스러운 Ragdoll 모션 연출이 가능했지만,
   
   **Capsule Component는 해당 시점에 고정되어 어색한 타격 판정**이 발생.
   
   -> Unreal 구조상 Simulate Physics가 활성화되면 해당 **Mesh가 위치 제어의 주체**가 되며 **Capsule Component는 물리 연산에서 제외**되기 때문.

   <br>
   <img src="https://github.com/user-attachments/assets/0b5d09e6-f828-4ba2-afe5-4ea6e1486611">
   <br>
   <br>
   
   => **Capsule Component**가 **Mesh**를 따라가야 하기 때문에 Tick보단 적은 비용의 Timer를 활용하여 Capsule Component의 위치를 변경함.

   <br>
   <img src="https://github.com/user-attachments/assets/9072ca3c-f612-4061-ae66-4424405e05e6">
   <br>
   
  </details>

## 인벤토리
[InventoryComponent.cpp](Source/Don/Private/Inventory/InventoryComponent.cpp)<br>

<img src="https://github.com/user-attachments/assets/b726ab54-e899-413e-a8a4-827a0a1a429a" alt="인벤토리 설명"/>

 
## 보스 AI
[BTTask_Rampage_UseAbility.cpp](Source/Don/Private/AI/BTTask/BTTask_Rampage_UseAbility.cpp)<br>

<img src="https://github.com/user-attachments/assets/822a05aa-6a38-4913-b1a3-80599c6eb19b">
<img src="https://github.com/user-attachments/assets/a915becc-cc7d-4db6-8e4a-f4d56a5510bd">

## 아이템 구조
[ItemStructs.h](Source/Don/Public/Data/ItemStructs.h)<br>

<img width="669" height="807" alt="image" src="https://github.com/user-attachments/assets/e4ad3b4a-c63f-4ecb-bb3c-bfd43df9a45b" />


</details>

<details>
 <summary>여관 경영 시뮬레이션</summary>

 <img width="1648" height="884" alt="image" src="https://github.com/user-attachments/assets/f271388d-d782-49f8-a861-922ac5974769" />

 # [포트폴리오 영상](https://youtu.be/_K6ISF4AflY)
 
 # 게임 장르
 여관 경영 시뮬레이션

 # 목차
 * [게임 소개](#게임-소개)
 * [낮 사이클](#낮-사이클)
 * [밤 사이클](#밤-사이클)
 * [손님 AI](#손님-ai)
 * [아키텍처(최적화 및 시스템 설계)](#아키텍처)
 * [트러블 슈팅](#트러블-슈팅)

 
 # 게임 소개 

 낮에는 요리와 숙소 서비스를 이용해 돈을 벌고 밤에는 손님들을 약탈하는 게임입니다.

 <img width="725" height="711" alt="image" src="https://github.com/user-attachments/assets/a5f6746a-1f74-4cb2-aaa9-1f22cb07b2ec" /><br><br>

 # 낮 사이클

 <img width="1684" height="948" alt="Image" src="https://github.com/user-attachments/assets/5a67b3df-9669-4993-a633-ffca5363b88e" /><br><br>

 <img width="1300" height="807" alt="image" src="https://github.com/user-attachments/assets/4a62b2b7-6840-41f7-8b90-9b50cd63b0b7" /><br><br>

 # 밤 사이클

 <img width="1652" height="931" alt="image" src="https://github.com/user-attachments/assets/98dd55db-5465-46d7-a51b-0098ded25e32" /><br><br>


 # 손님 AI

 [InnCustomer.cpp](Source/Don/Private/Inn/Character/InnCustomer.cpp)<br>
 
 <img width="1371" height="757" alt="image" src="https://github.com/user-attachments/assets/6024b8ab-5bfd-4692-913d-52157883edab" />

  1. 손님 그룹 입장
  2. 식당 빈 자리 착석
  3. 식사 완료 후 계산 + 숙소 체크인 or 퇴장
  4. 체크아웃 시간에 모든 손님 퇴장 및 계산


 # 아키텍처
 
 [InnManagerSubsystem.cpp](Don/Private/GameInstance/SubSystem/InnManagerSubsystem.cpp)<br>
 [KitchenOrderSubsystem.cpp](Source/Don/Private/GameInstance/SubSystem/KitchenOrderSubsystem.cpp)<br>

 <img width="1023" height="911" alt="image" src="https://github.com/user-attachments/assets/11219a71-ae14-439d-b15f-9d78eda72b9f" /><br><br>

 <img width="1312" height="789" alt="image" src="https://github.com/user-attachments/assets/1d779ac1-9978-4a44-a249-def9d616c577" /><br><br>



 ## Observer 패턴을 활용한 그룹 단위 상태 동기화

 [UCustomerGroup.cpp - OnMemberNotified()](Source/Don/Private/Inn/Object/InnCustomerGroup.cpp#L16)

 집단 지성 로직: **개별 멤버**(**AInnCustomer**)가 자신의 상태 변화를 **그룹**(**UInnCustomerGroup**)에 **통보**(**Notify**)하면, 그룹은 모든 멤버의 상태를 카운팅하여 '전원 입장', '전원 식사 완료' 같은 그룹 단위의 의사결정을 내립니다.

 이를 통해 개별 액터 간의 직접적인 참조를 줄여 스파게티 코드를 방지하고, 그룹의 다음 단계(좌석 요청, 숙소 입장 등)를 한 곳에서 안전하게 제어합니다.


 ## 가벼운 데이터 구조(Snapshot) 기반의 메모리 최적화

 [UCustomerGroup.cpp - MakeCustomerSnapshot()](Source/Don/Private/Inn/Object/InnCustomerGroup.cpp#L169)

 데이터와 로직의 분리: 손님이 숙소에 입장하여 화면에서 사라지는 순간, 무거운 AActor 객체를 유지하지 않고 필수 정보만 담은 **FCustomerSnapshot** 구조체로 변환합니다.

 리소스 절약: Destroy()를 호출해 월드 내 액터 수를 줄임으로써 Draw Call 및 AI(Behavior Tree) 업데이트 **오버헤드를 차단**합니다.

 데이터 보존: 액터는 사라지지만 핵심 데이터는 **UObject인 그룹**에 남겨, 밤의 던전 시스템에서 다시 활용할 수 있는 데이터 무결성을 확보했습니다.


 ## 트러블 슈팅

 <details>
  <summary>집단 행동의 의사결정 주체 설정</summary>

  1. 문제 상황 : 다수의 그룹 멤버들이 동일한 목적지로 이동하도록 **의사결정을 공유**해야 하는 상황 발생
     <br>=> 멤버마다 **동일한 그룹 ID**를 부여하여 개별 로직 처리를 유도했으나, 데이터의 **동기화 및 중복 연산** 문제 발생
     <br>=> 멤버 중 한 명을 **리더**로 설정했으나, 리더 액터의 예외 상황(**파괴**) 발생 시 전체 그룹의 의사결정 마비
     <br>=> 개별 액터의 판단을 기반으로 하기 때문에, 하나의 좌석에 같은 그룹의 멤버가 중복으로 앉으려고 시도하는 **경쟁 상태** 발생
  2. 과제 : 액터 파괴 시에 발생하는 불안정한 문제를 해결하고, 그룹의 **의사결정을 멤버들에게 동기화**시켜야 함
  3. 해결 방안 : 의사결정을 책임질 액터 대신에 UObject를 기반으로 한 **그룹 오브젝트**가 각각의 멤버들의 행동을 동기화시킴으로써 해결
  4. 결과 : 개별 액터의 판단으로 인해 발생하는 **논리적 충돌 제거** 및 의사결정 코드를 한 곳에 정리 가능

 </details>
</details>

<details>
 <summary>왜 하나의 Repository?</summary>
 처음에는 언리얼 C++ 시스템 구현 연습을 위한 기능 테스트 프로젝트로 시작했습니다.
 
 개발 과정에서 시스템 구조가 안정화되면서, 동일한 코드베이스를 활용해 장르가 다른 게임(여관 경영 / 액션)까지 확장하는 형태로 발전시켰습니다.
 
 이 과정에서 **공통 시스템을 재사용 가능한 구조로 개선하고, 장르가 달라져도 확장 가능한지 검증**하는 것을 목표로 했습니다.

 ## 하나의 프로젝트에서 두 장르를 구현하며 겪은 트러블슈팅

 ### 1. Character 의존적인 초기화 구조 개선
초기에는 "컨트롤러가 캐릭터를 Possess한다"는 전제 하에,  
`Character` 클래스에서 GAS 초기화 및 HUD 초기화를 수행하도록 설계했습니다.

 ```c++
void ADonCharacter::InitializeHUD()
{
	ADonPlayerState* DonPlayerState = GetPlayerState<ADonPlayerState>();
	DonPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(DonPlayerState, this);
	AbilitySystemComponent = DonPlayerState->GetAbilitySystemComponent();
	AttributeSet = DonPlayerState->GetAttributeSet();
	
	if (ADonPlayerController* DonPlayerController = Cast<ADonPlayerController>(GetController()))
	{
		if (ADonHUD* DonHUD = Cast<ADonHUD>(DonPlayerController->GetHUD()))
		{
			DonHUD->InitOverlay(DonPlayerController, DonPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}
 ```

하지만 여관 경영 모드에서는 캐릭터 메시가 필요하지 않아,
기본 Pawn/Character가 존재하지 않는 상황이 발생했고 이로 인해 초기화가 수행되지 않아 크래시가 발생했습니다.

**해결**: 캐릭터에 의존하던 HUD/GAS 초기화 책임을 `PlayerController`로 옮겨,
Pawn/Character 존재 여부와 관계없이 안정적으로 동작하도록 구조를 개선했습니다.

  ```c++
 void ADonPlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitializeHUD();
	RegisterUIBinding();	
}
 ```

### 2. 게임 모드별 UI 입력 충돌 해결

[UIManagerSubsystem.cpp](Source/Don/Private/UI/UIManagerSubsystem.cpp#L65)<br>
액션 모드에서는 WASD가 이동 입력이지만,
여관 모드에서는 WASD가 메뉴 이동/관리 UI 조작 등 다른 입력으로 동작할 가능성이 있었습니다.

이 확장성을 확보하기 위해,

* UI Manager를 통해 UI 제어를 중앙화하고
* 맵/게임 모드별로 다른 UI 동작을 UIConfigData(DataAsset)로 분리하여 관리했습니다.

 <img width="48%" height="800" alt="image" src="https://github.com/user-attachments/assets/bad8ad54-4874-410f-8b73-ed87be9fdc8a" />
 <img width="48%" height="800" alt="image" src="https://github.com/user-attachments/assets/a3116bff-7b6a-45bc-8d9b-9ded8ee16fe6" />

- 맵마다 필요한 UI 입력 바인딩 개수(예: 3개 / 2개)가 달라도 Config로 대응 가능
- 동일한 입력 키라도 맵에 따라 서로 다른 위젯을 매핑할 수 있도록 구성
 
</details>

[리팩토링 및 구조 설계 repo](https://github.com/Fsbsnw/UnrealPlayground)
