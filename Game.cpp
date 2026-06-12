#include "Game.h"
#include <cstdlib>
#include <ctime>
#include <string>

Game::Game() : window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Modular Smooth Snake"), state(MENU), score(0) {
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    if (!font.loadFromFile("arial.ttf")) {}
    initTexts();
}

void Game::initTexts() {
    menuTitleText = sf::Text("SNAKE GAME", font, 40);
    menuTitleText.setFillColor(sf::Color::Green);
    menuTitleText.setStyle(sf::Text::Bold);

    menuStartText = sf::Text("Press [ENTER] to Start\n\nPress [L] for Leaderboard", font, 18);
    menuStartText.setFillColor(sf::Color::White);

    scoreText = sf::Text("Score: 0", font, 18);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.0f, 10.0f);

    gameOverText = sf::Text("GAME OVER", font, 36);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setStyle(sf::Text::Bold);

    restartText = sf::Text("Press [SPACE] to Restart\n\nPress [M] for Main Menu", font, 16);
    restartText.setFillColor(sf::Color::Cyan);

    leaderboardTitleText = sf::Text("TOP 5 SCORES", font, 32);
    leaderboardTitleText.setFillColor(sf::Color::Yellow);
    leaderboardTitleText.setStyle(sf::Text::Bold);

    leaderboardBodyText = sf::Text("", font, 16);
    leaderboardBodyText.setFillColor(sf::Color::White);
}

void Game::restartGame() {
    snake.reset();
    score = 0;
    scoreText.setString("Score: 0");
    moveDuration = 0.15f;
    moveTimer = 0.0f;
    generateFood();
    state = PLAYING;
}

void Game::generateFood() {
    bool inSnake;
    do {
        inSnake = false;
        food.x = std::rand() % WIDTH;
        food.y = std::rand() % HEIGHT;
        for (const auto& part : snake.body) {
            if (food == part) { inSnake = true; break; }
        }
    } while (inSnake);
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed) {
            if (state == MENU) {
                if (event.key.code == sf::Keyboard::Enter) {
                    restartGame();
                }
                else if (event.key.code == sf::Keyboard::L) {
                    leaderboardBodyText.setString(dbManager.getTopScores());
                    state = LEADERBOARD;
                }
            }
            else if (state == LEADERBOARD) {
                if (event.key.code == sf::Keyboard::M || event.key.code == sf::Keyboard::Escape) {
                    state = MENU;
                }
            }
            else if (state == GAME_OVER) {
                if (event.key.code == sf::Keyboard::Space) {
                    restartGame();
                }
                else if (event.key.code == sf::Keyboard::M) {
                    state = MENU;
                }
            }
        }
    }
}

void Game::update(float deltaTime) {
    if (state != PLAYING) return;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    snake.changeDirection(UP);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))   snake.changeDirection(DOWN);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))   snake.changeDirection(LEFT);
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) snake.changeDirection(RIGHT);

    moveTimer += deltaTime;
    if (moveTimer >= moveDuration) {
        moveTimer = 0.0f;
        snake.dir = snake.nextDir;

        GridPos nextHead = snake.getNextHeadPos();

        if (nextHead.x < 0 || nextHead.x >= WIDTH || nextHead.y < 0 || nextHead.y >= HEIGHT || snake.checkSelfCollision(nextHead)) {
            state = GAME_OVER;
            dbManager.saveScore(score);
            return;
        }

        bool ateFood = (nextHead == food);
        snake.move(nextHead, ateFood);

        if (ateFood) {
            score += 10;
            scoreText.setString("Score: " + std::to_string(score));
            generateFood();
            if (moveDuration > 0.09f) moveDuration -= 0.01f;
        }
    }
}

void Game::render() {
    window.clear(sf::Color(30, 30, 30));

    if (state == MENU) {
        sf::FloatRect titleBounds = menuTitleText.getLocalBounds();
        menuTitleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
        menuTitleText.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 60);

        sf::FloatRect startBounds = menuStartText.getLocalBounds();
        menuStartText.setOrigin(startBounds.left + startBounds.width / 2.0f, startBounds.top + startBounds.height / 2.0f);
        menuStartText.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 40);

        window.draw(menuTitleText);
        window.draw(menuStartText);
    }
    else if (state == LEADERBOARD) {
        sf::FloatRect titleBounds = leaderboardTitleText.getLocalBounds();
        leaderboardTitleText.setOrigin(titleBounds.left + titleBounds.width / 2.0f, titleBounds.top + titleBounds.height / 2.0f);
        leaderboardTitleText.setPosition(WINDOW_WIDTH / 2.0f, 40.0f);

        sf::FloatRect bodyBounds = leaderboardBodyText.getLocalBounds();
        leaderboardBodyText.setOrigin(bodyBounds.left + bodyBounds.width / 2.0f, 0.0f);
        leaderboardBodyText.setPosition(WINDOW_WIDTH / 2.0f, 100.0f);

        sf::Text backText("Press [M] to return to Menu", font, 14);
        backText.setFillColor(sf::Color::Cyan);
        sf::FloatRect backBounds = backText.getLocalBounds();
        backText.setOrigin(backBounds.left + backBounds.width / 2.0f, backBounds.top + backBounds.height / 2.0f);
        backText.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT - 30.0f);

        window.draw(leaderboardTitleText);
        window.draw(leaderboardBodyText);
        window.draw(backText);
    }
    else {
        sf::CircleShape foodShape(CELL_SIZE / 2.0f - 2);
        foodShape.setFillColor(sf::Color::Red);
        foodShape.setOrigin(CELL_SIZE / 2.0f - 2, CELL_SIZE / 2.0f - 2);
        foodShape.setPosition(food.x * CELL_SIZE + CELL_SIZE / 2.0f, food.y * CELL_SIZE + CELL_SIZE / 2.0f);
        window.draw(foodShape);

        float progress = (state == GAME_OVER) ? 0.0f : (moveTimer / moveDuration);
        snake.draw(window, progress, state == GAME_OVER);

        window.draw(scoreText);

        if (state == GAME_OVER) {
            gameOverText.setString("GAME OVER\nFinal Score: " + std::to_string(score));
            sf::FloatRect goBounds = gameOverText.getLocalBounds();
            gameOverText.setOrigin(goBounds.left + goBounds.width / 2.0f, goBounds.top + goBounds.height / 2.0f);
            gameOverText.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f - 40);

            sf::FloatRect rBounds = restartText.getLocalBounds();
            restartText.setOrigin(rBounds.left + rBounds.width / 2.0f, rBounds.top + rBounds.height / 2.0f);
            restartText.setPosition(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f + 60);

            sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
            overlay.setFillColor(sf::Color(0, 0, 0, 170));

            window.draw(overlay);
            window.draw(gameOverText);
            window.draw(restartText);
        }
    }
    window.display();
}

void Game::run() {
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        processEvents();
        update(deltaTime);
        render();
    }
}
