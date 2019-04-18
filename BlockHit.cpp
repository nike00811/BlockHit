#include<cstdio>
#include<cstdlib>
#define GLUT_DISABLE_ATEXIT_HACK
#include<GL\glut.h>
#include<windows.h>
#include<ctime>

int level = 1 ;
char lev[2] ;
float ball_x, ball_y;
float ball_vx, ball_vy;

float bar_x, bar_y;
float bar_vx, bar_vy;
float bar_width, bar_height;

float pole_x , pole_y ;
float pole_vx , pole_vy ;
float pole_width , pole_height ;

float block_x[100], block_y[100];
float block_width, block_height;
int block_cnt;

bool trigger_left_key, trigger_right_key;
bool trigger_up_key , trigger_down_key ;
int game_state;

const int GAME_STATE_PLAY = 0;
const int GAME_STATE_END = 1;
/* 判斷是否是在某個長方形裡面
 * 參數：
 *    - x ：測試x座標
 *    - y ：測試y座標
 *    - w ：長方形寬度
 *    - h ：長方形高度
 *    - cx：長方形中心x座標
 *    - cy：長方形中心y座標
 */
bool IsInBox(float x, float y, float w, float h, float cx, float cy)
{
//	if( x <= cx - w / 2 || x >= cx + w / 2 || y <= cy - h / 2 || y >= cy + h / 2 )
//    	return true ;
//    else
    	return false ;
}
/* 判斷座標是否為空
 * 參數：
 *    - x：測試x座標
 *    - y：測試y座標
 */
bool IsLegalPoint(float x, float y)
{
	for(int i = 0 ; i < block_cnt ; i++)
		if(( x >= block_x[i] - block_width / 2 )and( x <= block_x[i] + block_width / 2 )and( y >= block_y[i] - block_height / 2 )and( y <= block_y[i] + block_height / 2 ))
			return false;
	if(( x < -1 )||( x > 1 )||( y > 1 )||( x >= bar_x - bar_width / 2 )&&( x <= bar_x + bar_width / 2 )&&( y >= bar_y - bar_height )&&( y <= bar_y + bar_height ))
		return false;
	else
    	return true;
}
/* 消除被撞到的磚塊
 * 參數：
 *    - x：x座標
 *    - y：y座標
 */
void BlockDelete(float x, float y)
{
	for(int i = 0 ; i < block_cnt ; i++)
		if( x >= block_x[i] - block_width / 2 && x <= block_x[i] + block_width / 2 && y >= block_y[i] - block_height / 2 && y <= block_y[i] + block_height / 2)
		{
			for(int j = i ; j < block_cnt - 1 ; j++)
			{
				block_x[j] = block_x[j+1] ;
				block_y[j] = block_y[j+1] ;
			}
			block_cnt-- ;
		}
	if( block_cnt == 0 )
	{
		level++ ;
		for(int lx = 0 ; lx < 5 ; lx++)
		{
			for(int ly = 0 ; ly < 6 ; ly++)
    		{
            	block_x[lx*6 + ly] = -0.5 + ly*0.2 ;
   	         	block_y[lx*6 + ly] = 0.5 - lx*0.1 ;
   	 	    }
   	 	}
   	 	block_cnt = 30 ;
		srand(time(NULL)) ;
		int t = rand() % 6 + 15 ;
		for(int k = 0 ; k < t ; k++)
   			for(int i = 0 ; i < block_cnt ; i++)
			{
				int u = rand() % (30 - k) ;
				if( i == u )
				{
					for(int j = i ; j < block_cnt - 1 ; j++)
					{
						block_x[j] = block_x[j+1] ;
						block_y[j] = block_y[j+1] ;
					}
					block_cnt-- ;
				}
			}
	}
	return;
}


void SystemTimer(int value)
{
    if(game_state == GAME_STATE_PLAY)
	{
        ball_x += ball_vx;
        ball_y += ball_vy;
        if(IsLegalPoint(ball_x + ball_vx, ball_y) == false)
		{
            BlockDelete(ball_x + ball_vx, ball_y);
            ball_vx = -ball_vx;
        }
        
        if(IsLegalPoint(ball_x, ball_y + ball_vy) == false)
		{
            BlockDelete(ball_x, ball_y + ball_vy);
            ball_vy = -ball_vy;
        }
        
        if(trigger_left_key && bar_x > -1.10 ) bar_x -= bar_vx; //桿子
        if(trigger_right_key && bar_x < 1.10 ) bar_x += bar_vx; //
        if(trigger_up_key && bar_y < -0.3 ) bar_y += bar_vy;    //
        if(trigger_down_key && bar_y > -0.6  ) bar_y -= bar_vy; //
        
        if(trigger_left_key && pole_x > -1.10 ) pole_x -= pole_vx;//紅色桿子 
        if(trigger_right_key && pole_x < 1.10 ) pole_x += pole_vx;// 
        if(trigger_up_key && pole_y > 0.6 ) pole_y -= pole_vy;//
        if(trigger_down_key && pole_y < 0.9  ) pole_y += pole_vy;// 
        if( level == 11 ) game_state = GAME_STATE_END; //過關  
		if(ball_y < -1) game_state = GAME_STATE_END;
		if(ball_x >= pole_x - pole_width / 2 && ball_x <= pole_x + pole_width / 2 && ball_y >= pole_y - pole_height && ball_y <= pole_y + bar_height )
			game_state = GAME_STATE_END;
    }
	else if(game_state == GAME_STATE_END)
	{
        //doing nothing
    }

    glutPostRedisplay();
    glutTimerFunc(25, SystemTimer, 1);
    return;
}


void DrawRectangle(float midx, float midy,float width, float height,
					float r, float g, float b)
{
	glBegin(GL_QUADS);
	glColor3f(r, g, b);
	glVertex3f(midx - width/2, midy - height/2, 0.0);
	glVertex3f(midx + width/2, midy - height/2, 0.0);
	glVertex3f(midx + width/2, midy + height/2, 0.0);
	glVertex3f(midx - width/2, midy + height/2, 0.0);
	glEnd();
	return;
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

    if(game_state == GAME_STATE_PLAY)
	{
		float wid = 0.05;
		DrawRectangle(ball_x, ball_y, wid, wid, 0.5, 0.5, 0.5);
		DrawRectangle(bar_x, bar_y, bar_width, bar_height, 0.5, 0.7, 0.7);
		DrawRectangle(pole_x, pole_y, pole_width, pole_height, 1, 0, 0);
		for(int lx = 0;lx < block_cnt ;lx++)
			DrawRectangle(block_x[lx], block_y[lx], block_width, block_height, 0.5, 0.3, 0.3);

    }
	else if(game_state == GAME_STATE_END)
	{
        // print GAME_END
        glColor3f(1,1,0); 
        float x = -0.4, y = 0;
        char str[] = "GAME OVER LEVEL: ";
        char win[] = "Congratulation!!";
        int a = level%10 ;  	//level
			lev[0] = a+48 ;
        strcat( str , lev ) ;
        
        if( level == 11 )  // 過關 
        	strcpy( str , win ) ;
        	
        glRasterPos2f((GLfloat)x,(GLfloat)y);
        for(int c = 0; str[c] != 0; c++)
            glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[c]);
    }
    
    glFlush();
    return;
}

void Init()
{
    bar_x = 0, bar_y = -0.5;
    bar_vx = 0.05, bar_vy = 0.01;
    bar_width = 0.3, bar_height = 0.015;
    
	pole_x = 0 , pole_y = 0.9 ;
	pole_vx = 0.05 , pole_vy = 0.01 ;
	pole_width = 0.3 , pole_height = 0.015 ;

    ball_vx = 0.01, ball_vy = 0.012;
    ball_x = 0, ball_y = 0;

    trigger_left_key = false, trigger_right_key = false;
    trigger_up_key = false, trigger_down_key = false;
    block_width = 0.18, block_height = 0.072;
    block_cnt = 6;
    for(int lx = 0 ; lx < 1 ; lx++)
        for(int ly = 0 ; ly < 6 ; ly++)
            block_x[lx*6 + ly] = -0.5 + ly*0.2 ,
            block_y[lx*6 + ly] = 0.5 - lx*0.1;
    
    game_state = GAME_STATE_PLAY;

    return;
}

void SpecialKeyDown(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT ) trigger_left_key = true;
    if(key == GLUT_KEY_RIGHT ) trigger_right_key = true;
    if(key == GLUT_KEY_UP ) trigger_up_key = true;
    if(key == GLUT_KEY_DOWN ) trigger_down_key = true;
    glutPostRedisplay();
    return;
}

void SpecialKeyUp(int key, int x, int y)
{
    if(key == GLUT_KEY_LEFT ) trigger_left_key = false;
    if(key == GLUT_KEY_RIGHT ) trigger_right_key = false;
    if(key == GLUT_KEY_UP ) trigger_up_key = false;
    if(key == GLUT_KEY_DOWN ) trigger_down_key = false;
    glutPostRedisplay();
    return;
}

void NormalKeyDown(unsigned char c, int x, int y)
{
    glutPostRedisplay();
    return;
}

void NormalKeyUp(unsigned char c, int x, int y)
{
	switch (c)
   {
	case 'z'://+速 
	ball_vx *= 1.5;
	ball_vy *= 1.5;
	case 'x'://-速 
	ball_vx *= 0.9;
	ball_vy *= 0.9;
   }
    glutPostRedisplay();
    return;
}

int main(int argc, char* argv[])
{
    Init();
    
    glutInit(&argc, argv);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Blockhit");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA); 
    glutDisplayFunc(Display);

    glutTimerFunc(25, SystemTimer, 1);

    glutKeyboardFunc(NormalKeyDown);
    glutKeyboardUpFunc(NormalKeyUp);
    glutSpecialFunc(SpecialKeyDown);
    glutSpecialUpFunc(SpecialKeyUp);

    glEnable(GL_BLEND);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glutMainLoop();

    return 0;
}

