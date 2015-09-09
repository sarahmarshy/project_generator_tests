#include "mbed.h"
#include "LM75B.h"
#include "C12832.h"
#include "MMA7660.h"
#include "Beep.h"


// Using Arduino pin notation
C12832 lcd(D11, D13, D12, D7, D10);
//Accelerometer
MMA7660 MMA(D14,D15);
//Temp Sensor
LM75B sensor(D14,D15);
//Speaker
Beep beeper(D6);
//Red LED
PwmOut r (D5);
//Green LED
PwmOut g (D9);

void pong(int ball_x, int y_pos){
    //bottom of the paddle
    int bottom_y = y_pos+10;
    //ball's y_location
    int ball_y = 15;
    //ball moving right
    bool right = true;
    //ball moving down
    bool ball_down = true;
    while(true) {
        //refresh the position of ball and paddle
        lcd.line(120, y_pos, 120, bottom_y, 1);
        lcd.fillcircle(ball_x, ball_y, 3, 1);
        wait(.03);
        lcd.fillcircle(ball_x, ball_y, 3, 0);
        lcd.line(120, y_pos, 120, bottom_y, 0);
        
        //check if the ball has hit the paddle
        if(ball_x+3 == 120 && ball_y>y_pos && ball_y<bottom_y && right){
            right = false;//now it is moving left
            beeper.beep(500, 0.25);//beep 
            r = 1.0;//turn off the red LED
            g = 0.0;//turn on green
        }
        //check if the ball is behind the paddle
        if(ball_x >= 124){
            right = false;//now it is moving left
            beeper.beep(500, 0.25);//beep
            g = 1.0;//turn off green LED
            r = 0.0;//turn on red
        }
        //if it hits the left wall
        if(ball_x<=3){
            right = true;
        }
        //if it hits the top or bottom
        if(ball_y<=3 || ball_y >= 29){
            ball_down = not(ball_down);//reverse y direction   
        }
        if(right){
            ball_x+=2;//increase ball's x pos by 2 if moving right
        }
        else{
            ball_x-=2;//decrease vy 2 if left
        }
        if(ball_down){
            ball_y++;//move ball's y pos down 1(higher y value's toward bottom of LCD) if the ball is moving down 
        }
        else{
            ball_y--;//ball's y up 1 if ball moving up
        }
        //Check if accelerometer is tilted away and the paddle hasn't hit the top
        if(MMA.x() > 0 && y_pos > 0){
            y_pos-=3;//top of paddle moved up by 3   
            bottom_y = y_pos+10;//set the bottom of the paddle to 10 below top
        }
        else if(MMA.x() < 0 && y_pos < 21){//Check if accelerometer is tilted toward and the paddle hasn't hit the bottom
            y_pos+=3;//top of paddle moves down 3
            bottom_y = y_pos+10;//bottom of the paddle is 10 pixels below top
        }
    }
}
int main()
{
    //Turn LEDs off
    r = 1.0;
    g = 1.0;
    //clear the screen
    lcd.cls();
    lcd.locate(0,3);
    //Display the temp and countfown
    lcd.printf("Temp = %.1f C\nPong will begin in: 3...\n",sensor.temp());
    wait(1);
    lcd.cls();
    lcd.locate(0,3);
    lcd.printf("Temp = %.1f C\nPong will begin in: 2...\n",sensor.temp());
    wait(1);
    lcd.cls();
    lcd.locate(0,3);
    lcd.printf("Temp = %.1f C\nPong will begin in: 1...\n",sensor.temp());
    wait(1);
    lcd.cls();
    //start pong
    pong(3,0);
    
}
