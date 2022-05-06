/* communication protocol:
   setup1 and attached: 0 = blue, 1 = orange
   setup2: any change (flip bit) = reroll
   friend: 1 = add 1 to def, 0 = nothing
   enemy: 0 = nothing
   01xxx = melee attack
   10xxx = magic attack
   (xxx = strength of attack)
*/
/* battle plan:
   clicked hex picks str or mag and sends stat
   recipient compares stat (losing ties).
      if it loses, updates shield. if it is still alive, it picks
      a stat and sends back its own attack.
      if it wins, sends back its own corresponding stat
*/
/*armor booster plan:
   clicked hex, sends a 1 if it has positive def, decrements def
   recipient increments def if sum of stats < 6, otherwise, sends
   back a 1 to restore decremented def
*/
/*state transitions:
   team (start):
    - single click/ change teams, same state
    - double click/ go to gostats
    - value on neighbor changes between 0 and 1/ change to match, same state
    - value on neighbor changes to 2/ go to gotostats
   gostats:
    - any neighbors not in gostats, same state
    - no neighbors not in gostats, reroll, go to stats
   stats:
    - single click/ go to gostats
    - double click/ go to goteam
    - long press/ go to inert
    - neighbor newly sending ROLLING/ go to gostats
   inert:
    - single click/ go to moving
    - double click/ go to statflash
    - long press/ go to goteam
    - attach/ go to attach
    - GOTEAM message/ go to goteam
    - ROLLING message/ pass on message, same state, DO NOT reroll
    moving:
    - single click/ update moving status:
      - same state if moves remain
      - inert if not
    - double click/ go to statflash
    - multiclick/ go to inert
    - long press/ go to team
    attach:
    - go to friend or enemy depending on team read on face
    statflash:
    - go to inert after timeout
    friend:
    - single click/ set giving value if possible, same state
    - detach/ go to inert
    - long press/ go to team
    enemy:
    - single click/ start battle, same state or dead if dead
    - detach/ go to inert
    - long press/ go to team
    dead:
    - long press/ go to team
    goteam:
    - any neighbors not in goteam, same state
    - no neighbors not in goteam, go to team
*/

//GLOBALS
#define EMPTY -1
#define PASS 48
enum stat_t {STR, MAG, SPD, DEF};
enum signals_t {ORANGETEAM, BLUETEAM, INERTORANGE, INERTBLUE, ATTACHEDFRIEND, ATTACHEDENEMY, ROLLING, REROLLING, STATS, DEAD, GOTEAM, TEAMSWAP, RECEIPT2, ROLLWAKE, REDIE, GIFT, RECEIPT};
void (*render)();
void (*handleMessages)(byte);
void (*handleAllFacesChecked)();
void (*handleClick)();
void (*handleDoubleClick)();
void (*handleMultiClick)();
void (*handleLongPress)();
byte stats[5] = {0, 0, 0, 0, 6};
byte team = 0;
byte moves;
byte foeface;
byte advance;
byte numneighbors;
byte myturn;
Timer animtimer;
Color statcolors[5] = {RED, MAGENTA, YELLOW, GREEN, OFF};

//FUNC DECLS
void setup();
void renderTeam();
void renderInert();
void renderStats();
void renderMoving();
void renderFriend();
void renderEnemy();
void renderDead();
void handleTeamClick();
void handleStatsClick();
void handleInertClick();
void handleMovingClick();
void handleFriendClick();
void handleEnemyClick();
void handleDeadClick();
void handleTeamDoubleClick();
void handleStatsDoubleClick();
void handleInertDoubleClick();
void handleMovingMultiClick();
void handleTeamLongPress();
void handleStatsLongPress();
void handleTeamMessages(byte);
void handleGoStatsMessages(byte);
void handleGoStatsAllFacesChecked();
void handleGoTeamMessages(byte);
void handleGoTeamAllFacesChecked();
void handleRerollingMessages(byte);
void handleRerollingAllFacesChecked();
void handleTeamSwapMessages(byte);
void handleTeamSwapAllFacesChecked();
void handleInertMessages(byte);
void handleInertAllFacesChecked();
void handleFoEMessages(byte);
void handleFriendAllFacesChecked();
void handleEnemyAllFacesChecked();
void handleDeadMessages(byte);
void handleRollwakeMessages(byte);
void handleRollwakeAllFacesChecked();
void handleRedieMessages(byte);
void handleRedieAllFacesChecked();
void nop();

//UTILITY FUNCS
void rerollStats() {
  for (byte i = 0; i < 4; i++) stats[i] = 0;
  for (byte i = 0; i < 6; i++) stats[random(3)]++;
}

//RENDER FUNCS
void renderHiding(byte brightness) {
  setColor(dim(team ? BLUE : ORANGE, brightness - 64 + sin8_C( millis() / 10 ) / 4));
}
void renderTeam() {
  if (!animtimer.isExpired()) setColor(YELLOW);
  else {
    byte offset = 245 * (millis() / 500 & 1);
    FOREACH_FACE(f) {
      setColorOnFace(dim(team ? BLUE : ORANGE, f % 2 ? 255 - offset : 10 + offset), f);
    }
  }
}
void renderInert() {
  int16_t val = animtimer.getRemaining();
  if (val > 50) {
    renderStats();
  } else if (val > 0) {
    setColor(RED);
  } else {
    renderHiding(128);
  }
}

void renderStats() {
  byte index = 0;
  byte count = 0;
  FOREACH_FACE(f) {
    while (!count) {
      count = stats[index++];
    }

    setColorOnFace(statcolors[index - 1], f);
    count--;
  }
}

void renderMoving() {
  if (animtimer.getRemaining() > 50) {
    renderStats();
  } else if (!animtimer.isExpired()) {
    setColor(YELLOW);
  } else {
    setColor(team ? BLUE : ORANGE);
    setColorOnFace(dim(team ? BLUE : ORANGE, 64), (millis() / 150) % 6);
  }
}

void renderFriend() {
  int c = animtimer.getRemaining();
  renderHiding(128);
  setColorOnFace(dim(GREEN, 128), foeface);
  if (c > 0 && c < 500) {
    for (byte i = 0; i < c / 100; i++) {
      setColorOnFace(YELLOW, (foeface + i) % 6);
      setColorOnFace(YELLOW, (foeface - i + 6) % 6);
    }
  }
}

void renderEnemy() {
  if (!animtimer.isExpired()) {
    setColor(team ? BLUE : ORANGE);
    uint32_t millitime = millis() / 10;
    setColorOnFace(WHITE, (2 * millitime % 6) + millitime % 2);
  } else {
    renderHiding(128);
    setColorOnFace(dim(RED, 128), foeface);
  }
}

void renderDead() {
  uint16_t val = animtimer.getRemaining();
  if (val > 256) setColor(dim(RED, val / 4));
  else {
    setColor(dim(RED,64));
    uint32_t millitime = millis()/360%48;
    setColorOnFace(dim(team ? BLUE : ORANGE, 150-3*(millitime%8)), millitime / 8);
  }
}


//SINGLE CLICK FUNCS
void handleTeamClick() {
  setValueSentOnAllFaces(TEAMSWAP);
  handleMessages = &handleTeamSwapMessages;
  handleAllFacesChecked = &handleTeamSwapAllFacesChecked;
  advance = 0;
}
void handleStatsClick() {
  setValueSentOnAllFaces(REROLLING);
  handleMessages = &handleRerollingMessages;
  handleAllFacesChecked = &handleRerollingAllFacesChecked;
  advance = 0;
}
void handleInertClick() {
  if (isAlone()) {
    render = &renderMoving;
    handleClick = &handleMovingClick;
    handleMultiClick = &handleMovingMultiClick;
    moves = stats[SPD] + 1;
    animtimer.set(50);
  }
}
void handleMovingClick() {
  moves--;
  animtimer.set(50);
  if (!moves) {
    render = &renderInert;
    handleClick = &handleInertClick;
    handleMultiClick = &nop;
  }
}
void handleFriendClick() {
  if (stats[DEF]) {
    setValueSentOnAllFaces(GIFT);
    stats[DEF]--;
  } else {
    animtimer.set(550);
    setValueSentOnAllFaces(RECEIPT);
  }
}
void handleEnemyClick() {
  myturn = 1;
  byte stat = random(1);
  setValueSentOnFace((stat + 1) << 4 | stats[stat], foeface);
}
void handleDeadClick() {
  setValueSentOnAllFaces(ROLLWAKE);
  handleMessages = &handleRollwakeMessages;
  handleAllFacesChecked = &handleRollwakeAllFacesChecked;
  advance = 0;
  animtimer.set(1020);
}

//DOUBLECLICK FUNCS
void handleTeamDoubleClick() {
  //we're going to gostats, so we only need to set up the signal and register a handler
  setValueSentOnAllFaces(ROLLING);
  handleMessages = &handleGoStatsMessages;
  handleAllFacesChecked = &handleGoStatsAllFacesChecked;
  handleClick = &nop;
  handleDoubleClick = &nop;
  handleLongPress = &nop;
  advance = 0;
}
void handleStatsDoubleClick() {
  //similar to above, but to goteam
  setValueSentOnAllFaces(GOTEAM);
  handleMessages = &handleGoTeamMessages;
  handleAllFacesChecked = &handleGoTeamAllFacesChecked;
  handleClick = &nop;
  handleDoubleClick = &nop;
  handleLongPress = &nop;
  advance = 0;
}
void handleInertDoubleClick() {
  animtimer.set(2000);
}

//MULTICLICK FUNCS
void handleMovingMultiClick() {
  moves = 1;
  handleMovingClick();
}

//LONGPRESS FUNCS
void handleTeamLongPress() {
  render = &renderInert;
  handleClick = &handleInertClick;
  handleDoubleClick = &handleInertDoubleClick;
  handleLongPress = &setup;
  handleMessages = &handleInertMessages;
  handleAllFacesChecked = &handleInertAllFacesChecked;
  setValueSentOnAllFaces(team+2);
  animtimer.set(0);
  advance = 0;
  if (moves) {
    byte tempmoves = moves;
    handleClick();
    moves = tempmoves;
  }
}

void handleStatsLongPress() {
  handleTeamLongPress();
}

//MESSAGE FUNCS

void handleTeamMessages(byte f) {
  byte val = getLastValueReceivedOnFace(f);
  if (val == TEAMSWAP) {
    animtimer.set(50);
    setValueSentOnAllFaces(TEAMSWAP);
    handleMessages = &handleTeamSwapMessages;
    handleAllFacesChecked = &handleTeamSwapAllFacesChecked;
    advance = 0;
  } else if (val == ROLLING) {
    setValueSentOnAllFaces(ROLLING);
    handleMessages = &handleGoStatsMessages;
    handleAllFacesChecked = &handleGoStatsAllFacesChecked;
    advance = 0;
  }
}

void handleGoStatsMessages(byte f) {
  byte val = getLastValueReceivedOnFace(f);
  if (val != ROLLING && val != STATS) advance = 0;
}

void handleGoStatsAllFacesChecked() {
  for (byte i = 0; i < 6; i++) {
    stats[random(3)]++;
  }
  render = &renderStats;
  handleMessages = &handleStatsMessages;
  handleClick = &handleStatsClick;
  handleDoubleClick = &handleStatsDoubleClick;
  handleLongPress = &handleStatsLongPress;
  handleAllFacesChecked = &nop;
  setValueSentOnAllFaces(STATS);
  rerollStats();
}

void handleStatsMessages(byte f) {
  byte val = getLastValueReceivedOnFace(f);
  if (val == GOTEAM) {
    handleStatsDoubleClick();
  } else if (val == REROLLING) {
    setValueSentOnAllFaces(REROLLING);
    handleMessages = &handleRerollingMessages;
    handleAllFacesChecked = &handleRerollingAllFacesChecked;
    advance = 0;
  }
}

void handleGoTeamMessages(byte f) {
  byte val = getLastValueReceivedOnFace(f);
  if (val != GOTEAM && val > BLUETEAM) advance = 0;
}

void handleGoTeamAllFacesChecked() {
  render = &renderTeam;
  handleMessages = &handleTeamMessages;
  handleClick = &handleTeamClick;
  handleDoubleClick = &handleTeamDoubleClick;
  handleMultiClick = &nop;
  handleLongPress = &handleTeamLongPress;
  handleAllFacesChecked = &nop;
  setValueSentOnAllFaces(team);
}

void handleRerollingMessages(byte f) {
  byte val = getLastValueReceivedOnFace(f);
  if (val != REROLLING && val != ROLLING) advance = 0;
}

void handleRerollingAllFacesChecked() {
  setValueSentOnAllFaces(ROLLING);
  handleMessages = &handleGoStatsMessages;
  handleAllFacesChecked = &handleGoStatsAllFacesChecked;
}

void handleTeamSwapMessages(byte f) {
  byte val = getLastValueReceivedOnFace(f);
  if (val != TEAMSWAP && val != GOTEAM) advance = 0;
}

void handleTeamSwapAllFacesChecked() {
  setValueSentOnAllFaces(GOTEAM);
  team = !team;
  handleMessages = &handleGoTeamMessages;
  handleAllFacesChecked = &handleGoTeamAllFacesChecked;
}

void handleInertMessages(byte f) {
  foeface = f;
  if (getLastValueReceivedOnFace(f) == GOTEAM) {
    handleStatsDoubleClick();
  }
}

void handleInertAllFacesChecked() {
  if (numneighbors == 1) {
    byte val = getLastValueReceivedOnFace(foeface);
    if (val < INERTORANGE || val > ATTACHEDENEMY) return;
    handleDoubleClick = &nop;
    handleLongPress = &handleStatsDoubleClick;
    handleMessages = handleFoEMessages;
    animtimer.set(0);
    if (val == team+2 || val == ATTACHEDFRIEND) {
      render = &renderFriend;
      handleClick = &handleFriendClick;
      handleAllFacesChecked = &handleFriendAllFacesChecked;
      setValueSentOnAllFaces(ATTACHEDFRIEND);
    } else {
      render = &renderEnemy;
      handleClick = &handleEnemyClick;
      handleAllFacesChecked = &handleEnemyAllFacesChecked;
      setValueSentOnAllFaces(ATTACHEDENEMY);
    }
  }
}

void handleFoEMessages(byte f) {
  foeface = f;
  if (getLastValueReceivedOnFace(f) == GOTEAM) {
    handleStatsDoubleClick();
  }
}

void handleFriendAllFacesChecked() {
  if (numneighbors != 1) {
    handleTeamLongPress();
  } else if (didValueOnFaceChange(foeface)) {

    byte val = getLastValueReceivedOnFace(foeface);
    if (val == GIFT) {
      animtimer.set(500);
      byte sum = 0;
      for (byte i = 0; i < 4; i++) sum += stats[i];
      if (sum < 6) {
        stats[DEF]++;
        setValueSentOnAllFaces(RECEIPT);
      } else {
        setValueSentOnAllFaces(GIFT);
      }
      moves=0;
    } else if (val == RECEIPT) {
      animtimer.set(500);
      setValueSentOnAllFaces(RECEIPT2);
      moves=0;
    } else {
      setValueSentOnAllFaces(ATTACHEDFRIEND);
    }
  }
}

void handleEnemyAllFacesChecked() {
  if (numneighbors != 1) {
    handleTeamLongPress();
  }
  byte val = getLastValueReceivedOnFace(foeface);
  if (val == DEAD) {
    animtimer.set(200);
    myturn = 0;
  }
  if (numneighbors == 1 && didValueOnFaceChange(foeface)) {
    if (val == RECEIPT) {
      setValueSentOnAllFaces(RECEIPT2);
      moves=0;
    } else if (val == RECEIPT2) {
      if (myturn) {
        myturn = 0;
        setValueSentOnAllFaces(PASS);
      } else {
        setValueSentOnAllFaces(RECEIPT2);
      }
    } else if (val & PASS) {
      moves=0;
      if (val == PASS) {
        handleEnemyClick();
      } else {
        byte attacktype = (val >> 4) - 1;
        byte attackstrength = val & 7;
        if (stats[attacktype] > attackstrength) {
          setValueSentOnAllFaces(val & PASS | stats[attacktype]);
        } else {
          if (stats[DEF]) {
            stats[DEF]--;
            if (myturn) {
              setValueSentOnAllFaces(RECEIPT2);
            } else {
              setValueSentOnAllFaces(RECEIPT);
            }
          } else {
            myturn = 0;
            render = &renderDead;
            handleClick = &handleDeadClick;
            handleDoubleClick = &nop;
            handleMessages = &handleDeadMessages;
            handleAllFacesChecked = &nop;
            setValueSentOnAllFaces(DEAD);
            animtimer.set(1020);
          }
        }
      }
    }
  }
}

void handleDeadMessages(byte f) {
  if (getLastValueReceivedOnFace(f) == ROLLWAKE) {
    handleDeadClick();
  }
  handleInertMessages(f);
}

void handleRollwakeMessages(byte f) {
  if (getLastValueReceivedOnFace(f) == DEAD) advance = 0;
}

void handleRollwakeAllFacesChecked() {
  setValueSentOnAllFaces(REDIE);
  handleMessages = &handleRedieMessages;
  handleAllFacesChecked = &handleRedieAllFacesChecked;
}

void handleRedieMessages(byte f) {
  if (getLastValueReceivedOnFace(f) == ROLLWAKE) advance = 0;
}

void handleRedieAllFacesChecked() {
  if (!random(5)) handleTeamLongPress();
  else {
    handleMessages = &handleDeadMessages;
    handleAllFacesChecked = &nop;
    setValueSentOnAllFaces(DEAD);
  }
}


//NULL FUNCTION
void nop() {
  return;
}

//MAIN SETUP
void setup() {

  randomize();
  handleStatsDoubleClick();
  rerollStats();
  moves = 0;
  myturn = 0;
  animtimer.set(0);
}

//MAIN GAME LOOP
void loop() {
  advance = 0;
  numneighbors = 0;
  foeface = EMPTY;
  FOREACH_FACE(f) {
    advance++;
    if (!isValueReceivedOnFaceExpired(f)) {//a neighbor!
      numneighbors++;
      handleMessages(f);
    }
  }
  if (advance == FACE_COUNT) {
    handleAllFacesChecked();
  }

  if (buttonSingleClicked()) {
    handleClick();
  }

  if (buttonDoubleClicked()) {
    handleDoubleClick();
  }

  if (buttonMultiClicked()) {
    handleMultiClick();
  }

  if (buttonLongPressed()) {
    handleLongPress();
  }

  render();

}
