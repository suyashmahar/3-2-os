# Operating Systems
### Compilation
```shell
make all
```

# Solutions

## Dining Philosophers Problem
### Execution
```shell
./phil.out
```
### Sample output
```
Starting simulation...
0000: T|T|T|T|T|
0001: T|T|T|T|T|
0002: T|T|T|T|T|
0003: T.E.T|T|T|
0004: T.E.T.E.T|
0005: T.E.T.E.T|
0006: T.E.T.E.T|
0007: T.E.T.E.T|
0008: T.E.T.E.T|
0009: T.E.T.E.H|

Stats...
  id:    eat  think hungry
   0:      0     10      0
   1:      7      2      1
   2:      0     10      0
   3:      6      3      1
   4:      0      8      2
```

## Bounded Buffer Problem

### Execution
```shell
./bbsem_mult.out
```

### Sample Output
```
[  PRODUCER00] 1837697572 to the buffer location: 0
[  PRODUCER00] 1306174092 to the buffer location: 1
[  PRODUCER00]    845537050 to the buffer location: 2
[  PRODUCER00] 1543982981 to the buffer location: 3
[  PRODUCER02]    753873901 to the buffer location: 4
[  PRODUCER02] 1163658392 to the buffer location: 5
[  PRODUCER02]    548873279 to the buffer location: 6
[  PRODUCER02]    300847263 to the buffer location: 7
[  PRODUCER02] Exiting
[  PRODUCER00] 1756115757 to the buffer location: 8
[  PRODUCER00] Exiting
[  PRODUCER04] Exiting
[  PRODUCER01] 1038781179 to the buffer location: 9
[  PRODUCER01] Exiting
[  CONSUMER00] Exiting
[  CONSUMER01] Exiting
[  CONSUMER02] Exiting
[  CONSUMER03] Exiting
[  CONSUMER04] Exiting
[  CONSUMER05] Exiting
[  CONSUMER06] Exiting
[  CONSUMER07] Exiting
[  PRODUCER03] Exiting
[  CONSUMER08] Exiting
 *** Simulation completed, cancelling child threads ***
[  CONSUMER09] Exiting
 *** Printing hits of each producer and consumer ***
Producers:
   0: 5
   1: 1
   2: 4
   3: 0
   4: 0
Consumers:
   0: 0
   1: 0
   2: 0
   3: 0
   4: 0
   5: 0
   6: 0
   7: 0
   8: 0
   9: 0
Producer total: 10
Consumer total: 0
```

## Cigarette Smokers Problem
### Execution
```shell
./cigsmoker.out
```
### Sample Output
```
[       AGENT] Agent A executed
[    PUSHER_P] pushed
[    PUSHER_T] Pushed
[    SMOKER_M] smoked
[       AGENT] Agent C executed
[    PUSHER_T] Pushed
[    PUSHER_M] pushed
[    SMOKER_P] smoked
[       AGENT] Agent A executed
[    PUSHER_T] Pushed
[    PUSHER_P] pushed
[    SMOKER_M] smoked
[    PUSHER_M] pushed
[       AGENT] Agent B executed
[    PUSHER_P] pushed
[    SMOKER_T] smoked
[       AGENT] Agent C executed
[    PUSHER_M] pushed
[    PUSHER_T] Pushed
[    SMOKER_P] smoked
[       AGENT] Agent A executed
[    PUSHER_P] pushed
[    PUSHER_T] Pushed
[    SMOKER_M] smoked
[       AGENT] Agent B executed
[    PUSHER_P] pushed
[    PUSHER_M] pushed
[    SMOKER_T] smoked
[       AGENT] Agent C executed
[    PUSHER_T] Pushed
[    PUSHER_M] pushed
[    SMOKER_P] smoked
[       AGENT] Agent A executed
[    PUSHER_T] Pushed
[    PUSHER_P] pushed
[    SMOKER_M] smoked
[       AGENT] Agent B executed
[    PUSHER_M] pushed
[    PUSHER_P] pushed
[    SMOKER_T] smoked
[       AGENT] Agent C executed
[    PUSHER_T] Pushed
[  SIMULATION] Simulation completed!
```

## Sleeping Barber Problem
### Execution
```shell
./barber.out
```

### Sample Output
```
[    CUSTOMER] Entered the shop!
[    CUSTOMER] Entered the shop!
[      BARBER] Gave a haircut, q.length=1
[    CUSTOMER] Entered the shop!
[      BARBER] Gave a haircut, q.length=1
[    CUSTOMER] Entered the shop!
[      BARBER] Gave a haircut, q.length=1
[      BARBER] Gave a haircut, q.length=0
[    CUSTOMER] Entered the shop!
[      BARBER] Gave a haircut, q.length=0
[    CUSTOMER] Entered the shop!
[    CUSTOMER] Entered the shop!
[    CUSTOMER] Entered the shop!
[      BARBER] Gave a haircut, q.length=2
[      BARBER] Gave a haircut, q.length=1
[    CUSTOMER] Entered the shop!
[    CUSTOMER] Entered the shop!
[      BARBER] Gave a haircut, q.length=2
```

