# Don
 언리얼 게임 클라이언트 포트폴리오 - 'Don'

 <img width="409" height="429" alt="Image" src="https://github.com/user-attachments/assets/7191d90b-b254-4b12-9a9a-7bd03c5b7981" />
 <br>


## 1인 프로젝트 - 최도의

## 게임 장르 : 3D 뱀파이어 서바이벌류

<details>
 <summary>개발중(여관 경영)</summary>
 
 # 전체적인 흐름 설계
 
 1. 손님 그룹 입장
 2. 식당 빈 자리 착석
 3. 식사 완료 후 계산 + 숙소 체크인 or 퇴장
 4. 체크아웃 시간에 모든 손님 퇴장 및 계산

 **<비헤이비어 트리>**
  <br>
  <img width="1342" height="752" alt="Image" src="https://github.com/user-attachments/assets/3b44fde5-e7da-4984-93a2-7cb66f14ee9f" />

  <img width="1409" height="939" alt="Image" src="https://github.com/user-attachments/assets/1aa57389-e132-44d2-9e78-69bf8228ec9f" />

  <img width="431" height="591" alt="Image" src="https://github.com/user-attachments/assets/25a8dcf5-69dc-4654-b70f-91cc188176c9" />

  <img width="515" height="785" alt="Image" src="https://github.com/user-attachments/assets/24c1308f-f884-459b-850d-2223a5c9d46a" />

  <img width="658" height="485" alt="Image" src="https://github.com/user-attachments/assets/f3a365f9-b1a4-4c44-ae6f-a2aae346ecaf" />

  <img width="1215" height="686" alt="Image" src="https://github.com/user-attachments/assets/fe9f6ea5-c6cc-407d-8a3a-e40bf7c90f5b" />

  <img width="1624" height="740" alt="Image" src="https://github.com/user-attachments/assets/39b9be3e-976e-4830-bccf-7f7b46705099" />

  <img width="475" height="181" alt="Image" src="https://github.com/user-attachments/assets/dd98423e-b782-4d43-b511-c162c54628c8" />

  <img width="464" height="265" alt="Image" src="https://github.com/user-attachments/assets/3b872c10-b947-45da-ab42-ad19d4094379" />

  특정 시간 경과 후, 룸 서비스 요청 및 미니게임 실행

  <img width="1655" height="835" alt="Image" src="https://github.com/user-attachments/assets/05e6787e-07c8-42d2-813c-5184ba97ad97" />

  
</details>

## 기획 배경

 이 프로젝트는 3D 환경에서 **‘뱀파이어 서바이벌’** 특유의 대규모 적을 쓸어버리는 **핵 앤 슬래시 쾌감**과 매번 **새롭게 강해지는 로그라이크적 재미**를 구현하는 것을 목표로 시작되었습니다. <br>
 
 특히 언리얼 엔진이라는 **무거운 기능 기반의 환경**에서, 수많은 AI 오브젝트를 **효율적으로 관리하고 최적화**하는 방법에 대한 기술적 호기심이 더해져, **게임적 기획 목표**와 성능 분석 및 최적화라는 **기술적 목표**를 동시에 추구하는 과제로 발전하게 되었습니다.
 
## 포트폴리오 요약
* 최적화 시도들
* GAS 시스템 기반 Ability 구현
* 인벤토리(MVC패턴)
* 퀘스트
* 아이템
* 장비 강화
* 상점
* 적 AI(Behavior Tree)

## [Don 포트폴리오 영상](https://youtu.be/gn_eyMzty2I)

## 최적화 시도
**<문제 상황>** <br>
적이 **50마리 정도가 스폰됐을 때, 프레임이 73 FPS에서 39 FPS로 급락**하는 심각한 성능 저하가 발생

<br>

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
   커스텀 **BTTaskNode_MoveToLocationAndRepath** 노드 사용
⇒ **UpdateOverlaps Time, PerformOverlapQuery Time** 감소 <br>

<BTTaskNode_MoveToLocationAndRepath - TickTask>
<img width="722" height="398" alt="Image" src="https://github.com/user-attachments/assets/59248caa-4225-4829-bf49-e444c45d929f" /> <br><br>

**<중간 결과>** <br>
프레임 드랍의 주요 원인 4가지의 비용을 현저히 낮춤 <br> 
**39** FPS -> **60** FPS <br>
**25** ms -> **17** ms <br>
(오브젝트 **50개** 기준)
<br><br>

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

 ## 트러블 슈팅


  <details>
   <summary>많은 오브젝트가 겹쳐 있을 때 스킬 이펙트로 인한 프레임 스파이크</summary>
   <br>
   
   스킬 피격 이펙트가 오브젝트마다 발생하기 때문에, 오브젝트가 뭉쳐 있는 경우 순간적으로 **60 FPS**까지 떨어짐   

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

   => **85 FPS**까지 방어 가능

   
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

  <details>
   <summary>아이템 구조 설계</summary>
   <br>
   처음엔 아이템 구조체 안에 선언된 타입별 Gameplay Tag에 따라 장착/사용/기타 등으로 동작을 분기하여 처리했으나,
   
   아이템 **종류가 다양**해지고 **속성이 복잡**해지면서 기존 방식에 **구조적 한계**가 발생.


   **<기존 Item 구조체>**
   <br>
   <img src="https://github.com/user-attachments/assets/fb3ec98e-0375-4055-abb4-f8c6e3e32357">
   <br>

   하나의 구조체 안에 공통 속성과 개별 속성을 통합된 상태로 선언했기 때문에, 구분이 어렵고 확장될수록 더 비대한 구조를 가지게 됨.
   
   그로 인해 **가독성 저하, 불필요한 메모리 낭비** 등 유지보수의 어려움이 발생.

   예시) 아이템의 희귀도, 희귀도에 따른 추가 속성, 장전이 필요한 무기 등이 추가될수록 더욱 복잡해짐.

   **=> 구조체의 확장성과 효율성을 어떻게 개선할 것인가?**

   1. FItem 구조체에는 **공통 속성**만을 넣고, 개별 속성들을 각각의 **데이터 애셋**에서 **FName**으로 참조하여 사용

   <br>
   <img width="1390" height="637" alt="Image" src="https://github.com/user-attachments/assets/64542a80-7bc4-44bd-b60b-3189a4517cb5" />
   <br>

   **<아이템 구조 분리>**

   <img src="https://github.com/user-attachments/assets/b10e95fc-64c7-45bd-9cf0-6a4cb399e97f">
   
   ---
   
   2. 아이템 사용을 위한 **ItemBase** 부모 클래스를 생성하여 다형성 확보

   <br>
   <img width="1266" height="516" alt="Image" src="https://github.com/user-attachments/assets/bb133b8d-a4ed-4313-a355-ee5c9914cb02" />
   <br>

   **<아이템 사용 로직>**

   <img src="https://github.com/user-attachments/assets/926ff9e9-cf5c-41cf-8f3f-56a909335fa5">

   **<포션 사용>**

   <img src="https://github.com/user-attachments/assets/9e7a87e8-5fa9-420d-bfe4-11cea100529c">

   **<장비 장착>**

   <img src="https://github.com/user-attachments/assets/e3f85f43-ed80-4dcb-8351-d82a9c026fd1">

   **<아이템 사용>**

   <img src="https://github.com/user-attachments/assets/a72b6b3f-73fa-427e-a15a-8d72d5bab266">
   
  </details>

## 게임 모드 소개
![Image](https://github.com/user-attachments/assets/0dc82893-94c5-41f8-bbc4-038b61fd2ee1)

## 캐릭터 클래스 구조
![Image](https://github.com/user-attachments/assets/b2ac41e2-0385-4ba3-82ed-4d22c7710ec4)

## 플레이어 캐릭터

### <li>Attribute Set</li>
 <br>
 <img src="https://github.com/user-attachments/assets/1253504f-281a-406c-a0ed-75744f33b4cd" alt="Attribute Set 설명" width="500"/>
 <br>
 <br>
<details>
 <summary>Attribute Set 속성 설명</summary>
 
 <br>
 <li>Vigor -> 체력, 공격력 추가</li>
 <li>Agility -> 치명타 확률, 추가 치명타 데미지, 회피 확률</li>
 <li>Supernatural -> 스킬 쿨타임 감소, 적 추가 스폰 확률, 아이템 획득 비율, 경험치 획득 비율, 돈 획득 비율</li>
 
 ### 예시
 <br>
 <img src="https://github.com/user-attachments/assets/71c7ada2-2da0-4670-86b5-840d0b83ca95">
 
</details>

---

### <li>인벤토리</li>
 <br>
 <img src="https://github.com/user-attachments/assets/b726ab54-e899-413e-a8a4-827a0a1a429a" alt="인벤토리 설명"/>
 <br>
 <br><br><br>
 <img src="https://github.com/user-attachments/assets/bd287183-838c-4773-9c51-ea4b22f3e109">
 <br><br><br>

 <li>Model</li>
 <br>
 <img src="https://github.com/user-attachments/assets/db80e350-abc2-482f-b26f-09d0e4eb3667">
 <img src="https://github.com/user-attachments/assets/1d9ddf32-39a7-4399-8c5e-fa8f8b315b66">
 <img src="https://github.com/user-attachments/assets/748763eb-bbe9-459a-b1b1-1287bb69a59f">
 <br><br>
 <li>Controller</li>
 <br>
 <img src="https://github.com/user-attachments/assets/67566903-be56-4649-a794-abbb4df617b9">
 <br><br>
 <li>View</li>
 <br>
 <img src="https://github.com/user-attachments/assets/7db17656-4466-4864-9763-a9d221d9b8bc">
 
 ---

 ### <li>장비 강화</li>
 <img src="https://github.com/user-attachments/assets/1dba1ded-3221-49e8-a2a3-0aedba6c8479">
 <img src="https://github.com/user-attachments/assets/6a296361-4df0-41b2-a1be-fd603d99136a">
 <br>
 
 강화 : **Upgrade Crystal** 소지 시 강화 가능

 <img src="https://github.com/user-attachments/assets/078e691a-77e3-4e78-8f65-cf58cbcfc83e">

## NPC 클래스
 ### <li>상점 NPC</li>

 <img src="https://github.com/user-attachments/assets/4b5aecae-46e3-4d73-9828-eb35e1c141eb">
 
 상인 아이템 구매
 
 <img src="https://github.com/user-attachments/assets/0d8fdd18-a6d9-40a1-a38b-99f8f1568db9">
 
 플레이어 아이템 판매
 
 <img src="https://github.com/user-attachments/assets/34694690-d7ed-40ab-9d44-7a076546228f">
 <br>

 ---

 ### <li>퀘스트</li>

 <img src="https://github.com/user-attachments/assets/636849ed-f893-42cf-964f-162eb205a59a">

 퀘스트 완료 보상
 
 <img src="https://github.com/user-attachments/assets/68ce44d7-7d70-429f-a75d-1f5dd4b258f1">

## 적 클래스
 ### 일반(따라가기, 공격)
 <img src="https://github.com/user-attachments/assets/193b8254-4b01-41f5-9d36-25b6f2413198">
 <img src="https://github.com/user-attachments/assets/fd238d1e-22d4-4898-ae35-34660fdae529">
 
 ### 보스(따라가기, 원거리 공격, 근거리 공격)
 <img src="https://github.com/user-attachments/assets/822a05aa-6a38-4913-b1a3-80599c6eb19b">
 <br>

  #### 보스 AI
  <img src="https://github.com/user-attachments/assets/a915becc-cc7d-4db6-8e4a-f4d56a5510bd">
  

  <br><Gameplay Tag로 능력을 활성화>
  
  <img src="https://github.com/user-attachments/assets/e19511ea-bacb-400c-a70a-a06938277b64">
