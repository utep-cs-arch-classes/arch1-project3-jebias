
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include "init.h"
#include "buzzer.h"

// basic struction for the shapes in the game
struct Rectangles {
  u_char x;
  u_char y;
  u_char width;
  u_char height;
  u_int color;
} rectangle;

struct Rectangles paddle1 = {screenWidth/2,screenHeight-15,30,5,COLOR_GREEN}; // bottom paddle controlled with S1 and S2
struct Rectangles paddle2 = {screenWidth/2,15,30,5,COLOR_GREEN}; // top paddle controlled with S3 and S4
struct Rectangles ball = {50,100,5,5,COLOR_RED}; 
char player2score = '0'; 
char player1score = '0';
char player1wins = 0; // when score = 2
char player2wins = 0; // when score = 2
char gameover = 0; // when a player scores twice
char buzz = 0; //failed to implement

void paintBoard()
{
  drawString5x7(5,5,"score:",COLOR_ORANGE,COLOR_BLUE); // top score 
  drawString5x7(5,screenHeight-10,"score:",COLOR_ORANGE,COLOR_BLUE); // bottom score
  drawChar5x7(50,screenHeight-10,player1score,COLOR_ORANGE,COLOR_BLUE); // actual score for top
  drawChar5x7(50,5,player2score,COLOR_ORANGE,COLOR_BLUE); //actual score for bottom
  fillRectangle(paddle1.x,paddle1.y,paddle1.width,paddle1.height,paddle1.color); // draw bottom paddle player 1
  fillRectangle(paddle2.x,paddle2.y,paddle2.width,paddle2.height,paddle2.color); // draw top paddle player 2
  fillRectangle(ball.x,ball.y,ball.width,ball.height,ball.color); // draw the ball
  
}

// loop for moving the paddles
void play(){
  while (1) {
    u_int switches = p2sw_read(), i;
    char str[5];
    for (i = 0; i < 4; i++)
      str[i] = (switches & (1<<i)) ? '-' : '0'+i;
    str[4] = 0;
    //paddle1 moving left
    if(str[0]=='0'){
      if(paddle1.x > 5){
      fillRectangle(paddle1.x,paddle1.y,paddle1.width,paddle1.height,COLOR_BLUE);
      paddle1.x -= 1;
      paintBoard();
      }
    }

    //paddle1 moving right
    if(str[1]=='1'){
      if(paddle1.x < 95){
      fillRectangle(paddle1.x,paddle1.y,paddle1.width,paddle1.height,COLOR_BLUE);
      paddle1.x += 1;
      paintBoard();
      }
    }

    //paddle2 moving left
    if(str[2]=='2'){
      if(paddle2.x > 5){
      fillRectangle(paddle2.x,paddle2.y,paddle2.width,paddle2.height,COLOR_BLUE);
      paddle2.x -= 1;
      paintBoard();
      }
    }

    //paddle2 moving right
    if(str[3]=='3'){
      if(paddle2.x < 95){
      fillRectangle(paddle2.x,paddle2.y,paddle2.width,paddle2.height,COLOR_BLUE);
      paddle2.x += 1;
      paintBoard();
      }
    }
    if(player1score == '2' || player2score == '2'){
      gameover = '1';
      break;
    }
    
  }
  
}
u_char dirx = 0;
u_char diry = 0;

// move the ball
void update_ball()
{
  if(gameover == 0){
  //bounce off of the walls
  if(ball.x <= 5)
    dirx = 1;
  else if(ball.x > 120)
    dirx = 0;
  if(ball.y <= 10)
    diry = 1;
  else if(ball.y > 150)
    diry = 0;
  //bounce off of the paddles
  if(ball.x >= paddle1.x && ball.x < paddle1.x + 30 && ball.y + 5 == paddle1.y){
    diry = 0;
    buzz = 1;
  }
  if(ball.x >= paddle2.x && ball.x < paddle2.x + 30 && ball.y == paddle2.y + 5)
    diry = 1;
  fillRectangle(ball.x,ball.y,5,5,COLOR_BLUE);
  if(dirx == 1)
    ball.x += 1;
  else
    ball.x -= 1;
  if(diry == 1)
    ball.y += 1;
  else
    ball.y -= 1;

  // if a player scores 
  if(ball.y == 10){
    player1score += 1;
    if(player1score == '2'){
      player1wins = '1';
      
    }
  }
  if(ball.y == 151){
    player2score += 1;
    if(player2score == '2'){
      player2wins = '1';
     
    }
  
 }
  paintBoard();
  }
}

// end the game by clearing the screen and displaying the winner
void gameOver(){
  gameover = '1';
  clearScreen(COLOR_BLUE);
  while(gameover == '1'){
  
    drawString5x7((screenWidth/2)-20,screenHeight/2,"Game Over",COLOR_RED,COLOR_BLUE);
    if(player1score == '2')
      drawString5x7((screenWidth/2)-40,(screenHeight/2)+10,"Player 1 Wins!",COLOR_RED,COLOR_BLUE);
    else
      drawString5x7((screenWidth/2)-40,(screenHeight/2)+10,"Player 2 Wins!",COLOR_RED,COLOR_BLUE);
  }
}

void main()
{
  /*
  configureClocks();
  enableWDTInterrupts();
  lcd_init();
  clearScreen(COLOR_BLUE);
  p2sw_init(15);
  or_sr(0x8);
  */

  // the comment above is in init()
  init();
  paintBoard();
  

  static char state = 0;
  while(1){
  switch(state){
    // start and play game
  case 0:
    play();
    state = 1;
    break;

    // if game is over
  case 1:
    
    gameOver();
    break;
  }
  }
  
}

void __interrupt_vec(WDT_VECTOR) WDT()
{
  static short count = 0;
  static short count2 = 0;
  count ++;
  count2 ++;

  // move the ball
  if(count == 5 && gameover == 0){
    update_ball();
    paintBoard();
    count = 0;
  }

  //failed to implement
  if(count2 < 50 && buzz == 1){
    //buzzer_init();
  }

  //clean up
  if(count2 == 50 && gameover == 0){
    buzz = 0;
    fillRectangle(paddle1.x-100,paddle1.y,100,paddle1.height,COLOR_BLUE);
  fillRectangle(paddle1.x+30,paddle1.y,100,paddle1.height,COLOR_BLUE); 
  fillRectangle(paddle2.x-100,paddle2.y,100,paddle2.height,COLOR_BLUE);
  fillRectangle(paddle2.x+30,paddle2.y,100,paddle2.height,COLOR_BLUE);
  fillRectangle(70,5,50,10,COLOR_BLUE);
  count2 = 0;
  }
}
