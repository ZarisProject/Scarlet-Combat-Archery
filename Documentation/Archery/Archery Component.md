*Name in code:* `USC_Archery_Component'
*Parent class:* `UActor_Component`

StateMachine : 

USC_ArcheryState_Machine*

Draw() ->

void

Release() ->

void

CancelDraw() ->

void

GetCurrentDrawTime() ->

float

IsFullyDrawn() ->

bool

GetCooldownTime() ->

float

IsOnCooldown() ->

bool

GetCurrentCooldownTime() ->

float

CurrentDrawTimer : 

float

DrawTime : 

float

GetDrawTime() ->

float

Cooldown : 

float

CurrentCooldown :

float

fields

methods

delegates

OnShot()

OnCanceled()

GetCurrentState() -> FName