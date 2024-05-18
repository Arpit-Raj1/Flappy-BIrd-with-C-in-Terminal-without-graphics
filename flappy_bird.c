#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>

#define xSize 32
#define ySize 16
#define pipeCount 3
#define GREEN "\e[32m"  // ANSI code for green output
#define YELLOW "\e[33m" // ANSI code for yellow output
#define RED "\e[31m"    // ANSI code for red output
#define NC "\e[0m"      // ANSI code for uncolord output

int flap_term = 1; // Going to behave as a boolean for game over

typedef struct
{
    int x;
    int y;
} obj;

obj bird;
obj pipes[3];

void f_Make();  // Regulates the printing everything
void f_Hit();   // Checks whether the bird hits the pipes or the borders
void f_Pipes(); // for formation of new pipes

void f_Make()
{
    char buffer[5000];        // Variable to hold the buffer that will be drawn
    strcpy(buffer, "\e[17A"); // Code to move the cursor up 17 lines

    for (int y = 0; y <= ySize; y++) // Loop over each row
    {
        for (int x = 0; x <= xSize; x++) // Loop over each column
        {
            if // If its a screen edge
                (
                    y == 0 ||
                    y == ySize ||
                    x == 0 ||
                    x == xSize)
            {
                strcat(buffer, NC "[]"); // Add '[]' to the buffer in white
                continue;                // Move to the next column
            }

            for (int i = 0; i < pipeCount; i++) // Loop over each pipe
            {
                // If its the top or bottom pipe face
                if (pipes[i].x >= x - 1 &&
                    pipes[i].x <= x + 1 &&
                    (pipes[i].y >= y + 3 ||
                     pipes[i].y <= y - 3))
                {
                    strcat(buffer, GREEN "[]"); // Add '[]' to the buffer in green
                    goto bottom;                // Skip the rest of the tests
                }
            }

            // The next bit will simply draw the bird in yellow based on x,y offsets
            if (
                bird.y == y &&
                bird.x == x)
            {
                strcat(buffer, YELLOW ")>");
            }
            else if (
                bird.y == y &&
                bird.x == x + 1)
            {
                strcat(buffer, YELLOW "_(");
            }
            else if (
                bird.y == y &&
                bird.x == x + 2)
            {
                strcat(buffer, YELLOW " _");
            }
            else if (
                bird.y == y - 1 &&
                bird.x == x)
            {
                strcat(buffer, YELLOW ") ");
            }
            else if (
                bird.y == y - 1 &&
                bird.x == x + 1)
            {
                strcat(buffer, YELLOW "__");
            }
            else if (
                bird.y == y - 1 &&
                bird.x == x + 2)
            {
                strcat(buffer, YELLOW " \\");
            }
            else // If its non of the other parts
            {
                strcat(buffer, NC "  ");
            }
        bottom:; // The point moved to with "goto bottom;"
        }
        strcat(buffer, "\n"); // Append a new line to the buffer
    }
    printf(buffer); // Write the buffer
}

void f_Pipes()
{
    for (int i = 0; i < pipeCount; i++)
    {
        if (pipes[i].x == -1) // If pipes is off screen
        {
            // If i == 0 then the distance betwwen pipes[0] && pipes[2] should be 15
            // Otherwise it should be (i-1)th pipe
            if (i == 0)
            {
                pipes[i].x = pipes[2].x + 15;
            }
            else
            {
                pipes[i].x = pipes[i - 1].x + 15;
            }
            pipes[i].y = (rand() % 7) + 5; // Setting pipes height
        }
    }
}

void f_Hit()
{
    if (bird.y == 15 || bird.y < 1) // Bird touches the floor or the roof
    {
        flap_term = 0;
    }

    for (int i = 0; i < pipeCount; i++) // Checks whether the bird is in the area where pipes are not present
    {
        if (
            (bird.x > pipes[i].x - 2) &&
            (bird.x - 2 < pipes[i].x + 2) &&
            ((bird.y < pipes[i].y - 2) ||
             (bird.y > pipes[i].y + 1)))
        {
            flap_term = 0;
        }
    }
}

int main()
{
    int replay = 1;
    while (replay == 1)
    {
        system("cls");
        flap_term = 1;
        srand(time(NULL));
        system("title \"Not Flappy Bird\""); // Renames powershell tab
        bird.x = 10;
        bird.y = 10;

        for (int i = 0; i < pipeCount; i++)
        {
            pipes[i].x = 25 + 15 * i;      // Distance netween to pipes is 15
            pipes[i].y = (rand() % 7) + 5; // Distance between pipe y  is random from 5 to 11
        }

        int frame = 0; // Variable to hold the passed frames
        printf("Press SPACE / UP arrow to start\n");

        for (int i = 0; i <= ySize; i++)
        {
            printf("\n");
        }

        f_Make();
        system("pause>nul"); // Pauses until any key is pressed

        while (1)
        {
            if (flap_term != 1)
            {
                break;
            }
            if (GetAsyncKeyState(VK_SPACE) || GetAsyncKeyState(VK_UP)) // Bird moves up when spacebar or up arrow is pressed
            {
                bird.y -= 2;
            }

            if (GetAsyncKeyState(VK_ESCAPE))
            {
                break;
            }

            if (frame == 2)
            {
                bird.y++;
                for (int i = 0; i < 3; i++)
                {
                    pipes[i].x -= 1;
                }
                frame = 0;
            }

            f_Hit();
            f_Make();
            f_Pipes();
            frame++;
            Sleep(100);
        }
        printf("Game Over\n");
        system("pause>nul");

        printf("Enter '1' to replay and '0' to exit: ");
        scanf("%d", &replay);
    }

    return 0;
}
