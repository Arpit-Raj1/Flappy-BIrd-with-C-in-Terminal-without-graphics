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
#define NC "\e[0m"      // ANSI code for uncolord output

typedef struct
{
    int x;
    int y;
} obj;

obj bird;
obj pipes[3];

void Make();

void Hit();

void Pipes();

void Make()
{
    char buffer[5000];        // Variable to hold the buffer that will be drawn
    strcpy(buffer, "\e[17A"); // ANSI code to move the cursor up 17 lines

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

        bottom: // The point moved to with "goto bottom;"
        }

        strcat(buffer, "\n"); // Append a new line to the buffer
    }

    printf(buffer); // Write the buffer
}

void Pipes()
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

void Hit()
{
    if (bird.y == 15 || bird.y < 1) // Bird touches the floor
    {
        exit(0);
    }

    for (int i = 0; i < pipeCount; i++)
    {
        if (
            (bird.x > pipes[i].x - 2) &&
            (bird.x - 2 < pipes[i].x + 2) &&
            ((bird.y < pipes[i].y - 2) ||
             (bird.y > pipes[i].y + 1)))
        {
            exit(0);
        }
    }
}

int main()
{
    srand(time(NULL));
    system("title \"Not Flappy Bird\"");

    bird.x = 10;
    bird.y = 10;

    for (int i = 0; i < pipeCount; i++)
    {
        pipes[i].x = 25 + 15 * i;      // Distance netween to pipes is 15
        pipes[i].y = (rand() % 7) + 5; // Distance between pipe y  is random from 5 to 11
    }

    int frame = 0; // Variable to hold the passed frames

    printf("Press SPACE to start\n");

    for (int i = 0; i <= ySize; i++)
    {
        printf("\n");
    }

    Make();

    system("pause>nul"); // Pauses until any key is pressed

    while (1)
    {
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

        Hit();

        Make();

        Pipes();

        frame++;

        Sleep(50);
    }

    return 0;
}
