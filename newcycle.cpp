#include <graphics.h>
#include <conio.h>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>

// Define M_PI if it's not defined
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using namespace std;

// Function to solve the Knapsack problem using dynamic programming
int knapsack(int capacity, vector<int>& weights, vector<int>& values, int n, vector<vector<int>>& dp) {
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= capacity; w++) {
            if (weights[i - 1] <= w) {
                dp[i][w] = max(values[i - 1] + dp[i - 1][w - weights[i - 1]], dp[i - 1][w]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    return dp[n][capacity];
}

// Function to find selected items
vector<int> findSelectedItems(int capacity, vector<int>& weights, vector<int>& values, int n, vector<vector<int>>& dp) {
    vector<int> selectedItems;

    int w = capacity;
    for (int i = n; i > 0 && w > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            selectedItems.push_back(i - 1);
            w -= weights[i - 1];
        }
    }

    return selectedItems;
}

void drawBackground(int offsetX) {
    // Draw the sky
    setfillstyle(SOLID_FILL, LIGHTBLUE);
    bar(0, 0, getmaxx(), 500); // Draw sky above the ground line

    // Draw the grass
    setfillstyle(SOLID_FILL, GREEN);
    bar(0, 500, getmaxx(), getmaxy()); // Draw grass below the ground line

    // Draw the sun
    setcolor(YELLOW);
    setfillstyle(SOLID_FILL, YELLOW);
    fillellipse(getmaxx() - 100 - offsetX, 100, 50, 50); // Draw the sun in the sky

    // Draw trees as background elements
    for (int i = 0; i < getmaxx(); i += 300) {
        int x = (i - offsetX) % (getmaxx() + 300) - 150;
        if (x > -100 && x < getmaxx()) {
            int treeBase = 450;
            int treeHeight = 100;
            setcolor(BROWN);
            setfillstyle(SOLID_FILL, BROWN);
            bar(x, treeBase - treeHeight, x + 20, treeBase); // Trunk
            setcolor(GREEN);
            setfillstyle(SOLID_FILL, GREEN);
            fillellipse(x + 10, treeBase - treeHeight, 50, 70); // Leaves
        }
    }
}

void drawCycle(int color, int circle1x, int circle2x, int poly_leftline, int poly_rightline, int poly_upline, int poly_lowerline, int handle_1, int handle_2, int handle_3, int sheet_1, int sheet_2, int wheelRotationAngle) {
    // Cycle tires with spokes
    setcolor(BLACK);
    for (int i = 0; i < 3; i++) {
        circle(circle1x, 450, 50 - i);    // first circle left side
        circle(circle2x, 450, 50 - i);    // second circle right side
    }
    for (int angle = 0; angle < 360; angle += 30) {
        int rotatedAngle1 = angle + wheelRotationAngle;
        int rotatedAngle2 = angle - wheelRotationAngle;
        line(circle1x, 450, circle1x + 50 * cos(rotatedAngle1 * M_PI / 180), 450 + 50 * sin(rotatedAngle1 * M_PI / 180));
        line(circle2x, 450, circle2x + 50 * cos(rotatedAngle2 * M_PI / 180), 450 + 50 * sin(rotatedAngle2 * M_PI / 180));
    }

    // Cycle frame
    setcolor(color);
    setlinestyle(SOLID_LINE, 1, 3);
    line(poly_lowerline, 450, poly_lowerline + 75, 450); // lower line of cycle
    line(poly_leftline, 450, poly_leftline + 50, 390); // left line of cycle
    line(poly_upline, 390, poly_upline + 75, 390); // upper line of cycle
    line(poly_rightline, 390, poly_rightline - 50, 450); // right line of cycle

    // Cycle handle
    line(handle_1, 450, handle_1 - 25, 390);
    line(handle_2, 390, handle_2 - 25, 340);
    line(handle_3, 340, handle_3 - 20, 340);

    // Cycle seat
    line(sheet_1, 390, sheet_1, 380);
    line(sheet_2, 380, sheet_2 + 10, 380);

    // Cycle pedals
    line(circle1x, 450, circle1x, 430); // left pedal
    line(circle2x, 450, circle2x, 430); // right pedal
}

void displayKnapsackResults() {
    // Example data for knapsack
    vector<int> weights = {2, 3, 4, 5};
    vector<int> values = {3, 4, 5, 6};
    int capacity = 5;
    int n = weights.size();

    // Initialize dp array for storing knapsack values
    vector<vector<int>> dp(n + 1, vector<int>(capacity + 1, 0));

    // Solve the knapsack problem
    int maxValue = knapsack(capacity, weights, values, n, dp);

    // Find selected items
    vector<int> selectedItems = findSelectedItems(capacity, weights, values, n, dp);

    // Display the results on the screen
    setcolor(WHITE);
    settextstyle(0, HORIZ_DIR, 2);
    outtextxy(50, 50, "Knapsack Results:");
    
    char result[50];
    sprintf(result, "Max Value: %d", maxValue);
    outtextxy(50, 80, result);

    outtextxy(50, 110, "Selected Items:");
    int yPosition = 140;
    for (int i : selectedItems) {
        sprintf(result, "Item %d (Weight: %d, Value: %d)", i, weights[i], values[i]);
        outtextxy(50, yPosition, result);
        yPosition += 30;
    }
}

int main() {
    // MOVING CYCLE PROGRAM WITH ARROW KEYS & CHANGING COLOURS WITH FUNCTION KEYS---

    // CONTROLS:
    // Cycle color change: (F1-F9)
    // Cycle movements: (right arrow, left arrow)
    // Display knapsack solution: (K key)

    // Declaration 
    int h = 0, page = 0;
    int circle1x = 100, circle2x = 250;
    int poly_upline = 150, poly_lowerline = 100, poly_leftline = 100, poly_rightline = 225;
    int handle_1 = 250, handle_2 = 225, handle_3 = 200;
    int sheet_1 = 150, sheet_2 = 145;
    int speed = 5;
    int n = 0;
    int Cycle_Color = WHITE;
    int backgroundOffsetX = 0;
    int wheelRotationAngle = 0;

    // Creating Graphics Window
    DWORD screenwidth = GetSystemMetrics(SM_CXSCREEN);
    DWORD screenHeight = GetSystemMetrics(SM_CYSCREEN);
    initwindow(screenwidth, screenHeight, "Cool Programming Projects", -3);

    char s[30] = "Loading...";
    while (1) {
        if (n == 0) {
            setcolor(WHITE);
            rectangle(575, 320, 725, 340);
            for (int i = 0; i < 148; i++) {
                setcolor(GREEN);
                line(576 + i, 321, 576 + i, 339);
                setcolor(WHITE);
                outtextxy(615, 350, s);
                delay(10);
            }
        }

        if (n != 0) {
            setactivepage(page);
            setvisualpage(1 - page);
        }
        cleardevice();

        // Draw the background with the current offset
        drawBackground(backgroundOffsetX);

        setcolor(WHITE);
        line(0, 500, getmaxx(), 500);

        // Draw the cycle with rotating wheels
        drawCycle(Cycle_Color, circle1x, circle2x, poly_leftline, poly_rightline, poly_upline, poly_lowerline, handle_1, handle_2, handle_3, sheet_1, sheet_2, wheelRotationAngle);

        if (n == 0) {
            setcolor(GREEN);
            settextstyle(0, HORIZ_DIR, 2);
            outtextxy(440, 570, "Press Any Key To Continue...");
            getch();
            n++;
        }

        if (n == 1) {
            setcolor(RED);
            rectangle(550, 570, 600, 620);
            rectangle(549, 571, 599, 619);
            rectangle(548, 572, 598, 618);
            setcolor(RED);
            line(560, 595, 590, 595);
            line(560, 594, 590, 594);
            line(560, 596, 590, 596);
            int points[8] = {560, 595, 570, 590, 570, 600, 560, 595};
            setfillstyle(SOLID_FILL, RED);
            fillpoly(4, points);

            rectangle(720, 570, 770, 620);
            rectangle(719, 571, 769, 619);
            rectangle(718, 572, 768, 621);
            line(730, 595, 760, 595);
            line(730, 596, 760, 596);
            line(730, 594, 760, 594);
            int points2[8] = {760, 595, 750, 590, 750, 600, 760, 595};
            fillpoly(4, points2);
        }

        // Handle left arrow key press
        if (GetAsyncKeyState(VK_LEFT)) {
            backgroundOffsetX -= speed;
            circle1x -= speed;
            circle2x -= speed;
            poly_upline -= speed;
            poly_lowerline -= speed;
            poly_leftline -= speed;
            poly_rightline -= speed;
            handle_1 -= speed;
            handle_2 -= speed;
            handle_3 -= speed;
            sheet_1 -= speed;
            sheet_2 -= speed;
            wheelRotationAngle = (wheelRotationAngle - 10) % 360;
        }

        // Handle right arrow key press
        if (GetAsyncKeyState(VK_RIGHT)) {
            backgroundOffsetX += speed;
            circle1x += speed;
            circle2x += speed;
            poly_upline += speed;
            poly_lowerline += speed;
            poly_leftline += speed;
            poly_rightline += speed;
            handle_1 += speed;
            handle_2 += speed;
            handle_3 += speed;
            sheet_1 += speed;
            sheet_2 += speed;
            wheelRotationAngle = (wheelRotationAngle + 10) % 360;
        }

        // Handle function keys for changing cycle color
        if (GetAsyncKeyState(VK_F1)) Cycle_Color = WHITE;
        if (GetAsyncKeyState(VK_F2)) Cycle_Color = YELLOW;
        if (GetAsyncKeyState(VK_F3)) Cycle_Color = CYAN;
        if (GetAsyncKeyState(VK_F4)) Cycle_Color = MAGENTA;
        if (GetAsyncKeyState(VK_F5)) Cycle_Color = LIGHTBLUE;
        if (GetAsyncKeyState(VK_F6)) Cycle_Color = LIGHTGREEN;
        if (GetAsyncKeyState(VK_F7)) Cycle_Color = LIGHTRED;
        if (GetAsyncKeyState(VK_F8)) Cycle_Color = BLUE;
        if (GetAsyncKeyState(VK_F9)) Cycle_Color = RED;

        // Handle 'K' key to display knapsack results
        if (GetAsyncKeyState('K')) {
            cleardevice();
            drawBackground(backgroundOffsetX);
            displayKnapsackResults();
            delay(15000); // Display for 2 seconds before continuing
        }

        delay(10);
        page = 1 - page;
    }

    getch();
    closegraph();
    return 0;
}
