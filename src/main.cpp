#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <iostream>

using namespace sf;
using namespace std;

class Tetris {
public:
    Tetris() : window(VideoMode(400, 600), "Tetris"), gen(rd()) {
        initialize();
    }

    void run() {
        while (window.isOpen()) {
            processEvents();
            update();
            render();
        }
    }

private:
    // Constants
    static const int BOARD_WIDTH = 10;
    static const int BOARD_HEIGHT = 20;
    static const int CELL_SIZE = 30;
    
    // Game state
    vector<vector<int>> board;
    vector<vector<int>> currentPiece;
    int currentX, currentY;
    int currentColor;
    bool gameOver;
    int score;
    float fallSpeed;
    float fallTimer;
    
    // SFML
    RenderWindow window;
    Clock clock;
    Font font;
    
    // Pieces definitions
    vector<vector<vector<int>>> pieces;
    vector<Color> colors;
    
    // Random generation
    random_device rd;
    mt19937 gen;

    void initialize() {
        // Initialize board
        board.resize(BOARD_HEIGHT, vector<int>(BOARD_WIDTH, 0));
        
        // Define colors
        colors = {
            Color::Black,        // 0 - empty
            Color::Cyan,         // 1 - I
            Color::Blue,         // 2 - J
            Color(255, 165, 0),  // 3 - L (orange)
            Color::Yellow,       // 4 - O
            Color::Green,        // 5 - S
            Color::Magenta,      // 6 - T
            Color::Red           // 7 - Z
        };
        
        // Define pieces
        pieces = {
            // I piece
            {{0,0,0,0}, {1,1,1,1}, {0,0,0,0}, {0,0,0,0}},
            // J piece
            {{2,0,0}, {2,2,2}, {0,0,0}},
            // L piece
            {{0,0,3}, {3,3,3}, {0,0,0}},
            // O piece
            {{4,4}, {4,4}},
            // S piece
            {{0,5,5}, {5,5,0}, {0,0,0}},
            // T piece
            {{0,6,0}, {6,6,6}, {0,0,0}},
            // Z piece
            {{7,7,0}, {0,7,7}, {0,0,0}}
        };

        gameOver = false;
        score = 0;
        fallSpeed = 1.0f;
        fallTimer = 0;

        // Load font (fallback to default if not available)
        if (!font.loadFromFile("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf")) {
            cout << "Font not found, using default\n";
        }

        spawnPiece();
    }

    void processEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            
            if (event.type == Event::KeyPressed && !gameOver) {
                switch (event.key.code) {
                    case Keyboard::Left:
                        movePiece(-1, 0);
                        break;
                    case Keyboard::Right:
                        movePiece(1, 0);
                        break;
                    case Keyboard::Down:
                        movePiece(0, 1);
                        break;
                    case Keyboard::Up:
                        rotatePiece();
                        break;
                    case Keyboard::Space:
                        // Hard drop
                        while (movePiece(0, 1)) {}
                        mergePiece();
                        clearLines();
                        spawnPiece();
                        break;
                    case Keyboard::R:
                        if (gameOver) {
                            resetGame();
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

    void update() {
        if (gameOver) return;
        
        float deltaTime = clock.restart().asSeconds();
        fallTimer += deltaTime;
        
        if (fallTimer >= fallSpeed) {
            if (!movePiece(0, 1)) {
                mergePiece();
                clearLines();
                spawnPiece();
            }
            fallTimer = 0;
        }
    }

    void render() {
        window.clear(Color(30, 30, 30));
        
        drawBoard();
        drawPiece();
        drawUI();
        
        window.display();
    }

    void spawnPiece() {
        uniform_int_distribution<> pieceDist(0, pieces.size() - 1);
        uniform_int_distribution<> colorDist(1, colors.size() - 1);
        
        int pieceIndex = pieceDist(gen);
        currentPiece = pieces[pieceIndex];
        currentColor = colorDist(gen);
        
        currentX = BOARD_WIDTH / 2 - currentPiece[0].size() / 2;
        currentY = 0;
        
        if (!isValidMove(currentPiece, currentX, currentY)) {
            gameOver = true;
        }
    }

    bool isValidMove(const vector<vector<int>>& piece, int x, int y) {
        for (int row = 0; row < piece.size(); ++row) {
            for (int col = 0; col < piece[row].size(); ++col) {
                if (piece[row][col] != 0) {
                    int boardX = x + col;
                    int boardY = y + row;
                    
                    if (boardX < 0 || boardX >= BOARD_WIDTH || 
                        boardY >= BOARD_HEIGHT || 
                        (boardY >= 0 && board[boardY][boardX] != 0)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    void mergePiece() {
        for (int row = 0; row < currentPiece.size(); ++row) {
            for (int col = 0; col < currentPiece[row].size(); ++col) {
                if (currentPiece[row][col] != 0) {
                    int boardX = currentX + col;
                    int boardY = currentY + row;
                    if (boardY >= 0) {
                        board[boardY][boardX] = currentColor;
                    }
                }
            }
        }
    }

    void clearLines() {
        int linesCleared = 0;
        
        for (int row = BOARD_HEIGHT - 1; row >= 0; --row) {
            bool lineComplete = true;
            for (int col = 0; col < BOARD_WIDTH; ++col) {
                if (board[row][col] == 0) {
                    lineComplete = false;
                    break;
                }
            }
            
            if (lineComplete) {
                board.erase(board.begin() + row);
                board.insert(board.begin(), vector<int>(BOARD_WIDTH, 0));
                linesCleared++;
                row++;
            }
        }
        
        if (linesCleared > 0) {
            score += linesCleared * 100;
            fallSpeed = max(0.1f, fallSpeed - 0.05f);
        }
    }

    void rotatePiece() {
        if (currentPiece.empty()) return;
        
        vector<vector<int>> rotated(currentPiece[0].size(), vector<int>(currentPiece.size()));
        
        for (int i = 0; i < currentPiece.size(); ++i) {
            for (int j = 0; j < currentPiece[i].size(); ++j) {
                rotated[j][currentPiece.size() - 1 - i] = currentPiece[i][j];
            }
        }
        
        if (isValidMove(rotated, currentX, currentY)) {
            currentPiece = rotated;
        }
    }

    bool movePiece(int dx, int dy) {
        if (isValidMove(currentPiece, currentX + dx, currentY + dy)) {
            currentX += dx;
            currentY += dy;
            return true;
        }
        return false;
    }

    void resetGame() {
        board = vector<vector<int>>(BOARD_HEIGHT, vector<int>(BOARD_WIDTH, 0));
        gameOver = false;
        score = 0;
        fallSpeed = 1.0f;
        spawnPiece();
    }

    void drawBoard() {
        RectangleShape cell(Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
        
        for (int row = 0; row < BOARD_HEIGHT; ++row) {
            for (int col = 0; col < BOARD_WIDTH; ++col) {
                cell.setPosition(col * CELL_SIZE + 1, row * CELL_SIZE + 1);
                if (board[row][col] != 0) {
                    cell.setFillColor(colors[board[row][col]]);
                } else {
                    cell.setFillColor(Color(50, 50, 50));
                    cell.setOutlineThickness(1);
                    cell.setOutlineColor(Color(70, 70, 70));
                }
                window.draw(cell);
            }
        }
    }

    void drawPiece() {
        if (gameOver) return;
        
        RectangleShape cell(Vector2f(CELL_SIZE - 2, CELL_SIZE - 2));
        cell.setFillColor(colors[currentColor]);
        
        for (int row = 0; row < currentPiece.size(); ++row) {
            for (int col = 0; col < currentPiece[row].size(); ++col) {
                if (currentPiece[row][col] != 0) {
                    int x = (currentX + col) * CELL_SIZE + 1;
                    int y = (currentY + row) * CELL_SIZE + 1;
                    if (y >= 0) {
                        cell.setPosition(x, y);
                        window.draw(cell);
                    }
                }
            }
        }
    }

    void drawUI() {
        // Draw score
        Text scoreText;
        scoreText.setFont(font);
        scoreText.setString("Score: " + to_string(score));
        scoreText.setCharacterSize(20);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(10, BOARD_HEIGHT * CELL_SIZE + 10);
        window.draw(scoreText);

        // Draw controls
        Text controlsText;
        controlsText.setFont(font);
        controlsText.setString("Controls: Arrow Keys = Move, Up = Rotate, Space = Hard Drop");
        controlsText.setCharacterSize(14);
        controlsText.setFillColor(Color::White);
        controlsText.setPosition(10, BOARD_HEIGHT * CELL_SIZE + 40);
        window.draw(controlsText);

        // Draw game over message
        if (gameOver) {
            RectangleShape overlay(Vector2f(BOARD_WIDTH * CELL_SIZE, BOARD_HEIGHT * CELL_SIZE));
            overlay.setFillColor(Color(0, 0, 0, 150));
            window.draw(overlay);

            Text gameOverText;
            gameOverText.setFont(font);
            gameOverText.setString("GAME OVER\nPress R to Restart");
            gameOverText.setCharacterSize(24);
            gameOverText.setFillColor(Color::Red);
            gameOverText.setPosition(BOARD_WIDTH * CELL_SIZE / 2 - 80, BOARD_HEIGHT * CELL_SIZE / 2 - 30);
            window.draw(gameOverText);
        }
    }
};

int main() {
    Tetris game;
    game.run();
    return 0;
}
