#include <SFML/Graphics.hpp>

#include <iostream>
#include <optional>
#include <string>

namespace
{
constexpr int gridSize = 10;
constexpr float tileSize = 60.f;
constexpr sf::Vector2f gridOrigin{50.f, 50.f};
} // namespace

int main()
{
    sf::RenderWindow window(sf::VideoMode({1000u, 700u}), "SFML Sandbox", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.openFromFile("assets/fonts/tuffy.ttf"))
    {
        std::cerr << "Failed to load assets/fonts/tuffy.ttf\n";
        return 1;
    }

    sf::Texture texture;
    if (!texture.loadFromFile("assets/images/sample.png"))
    {
        std::cerr << "Failed to load assets/images/sample.png\n";
        return 1;
    }
    sf::Sprite sprite(texture);
    sprite.setPosition({720.f, 400.f});

    sf::Vector2i hero{0, 0};
    std::string clickInfo = "Click a tile";

    sf::Text title(font, "SFML 3.1 Sandbox", 28);
    title.setPosition({700.f, 50.f});

    sf::Text info(font, "", 20);
    info.setPosition({700.f, 110.f});

    sf::RectangleShape tile({tileSize, tileSize});
    tile.setOutlineThickness(-1.f);
    tile.setOutlineColor(sf::Color(30, 30, 30));

    sf::RectangleShape heroShape({tileSize - 16.f, tileSize - 16.f});
    heroShape.setFillColor(sf::Color(220, 50, 50));
    heroShape.setOutlineThickness(2.f);
    heroShape.setOutlineColor(sf::Color::White);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                switch (key->code)
                {
                case sf::Keyboard::Key::Escape: window.close(); break;
                case sf::Keyboard::Key::W: if (hero.y > 0) --hero.y; break;
                case sf::Keyboard::Key::S: if (hero.y < gridSize - 1) ++hero.y; break;
                case sf::Keyboard::Key::A: if (hero.x > 0) --hero.x; break;
                case sf::Keyboard::Key::D: if (hero.x < gridSize - 1) ++hero.x; break;
                default: break;
                }
            }
            else if (const auto* click = event->getIf<sf::Event::MouseButtonPressed>())
            {
                const sf::Vector2f pos = window.mapPixelToCoords(click->position) - gridOrigin;
                const int col = static_cast<int>(pos.x / tileSize);
                const int row = static_cast<int>(pos.y / tileSize);
                if (pos.x >= 0.f && pos.y >= 0.f && col < gridSize && row < gridSize)
                    clickInfo = "Clicked tile (" + std::to_string(col) + ", " + std::to_string(row) + ")";
                else
                    clickInfo = "Clicked outside grid";
                std::cout << clickInfo << '\n';
            }
        }

        info.setString("Hero: (" + std::to_string(hero.x) + ", " + std::to_string(hero.y) + ")\n" + clickInfo +
                       "\n\nW/A/S/D - move\nMouse - select tile\nEsc - quit");

        window.clear(sf::Color(20, 20, 35));

        for (int row = 0; row < gridSize; ++row)
        {
            for (int col = 0; col < gridSize; ++col)
            {
                tile.setFillColor((row + col) % 2 == 0 ? sf::Color(70, 140, 70) : sf::Color(90, 170, 90));
                tile.setPosition(gridOrigin + sf::Vector2f(col * tileSize, row * tileSize));
                window.draw(tile);
            }
        }

        heroShape.setPosition(gridOrigin + sf::Vector2f(hero.x * tileSize + 8.f, hero.y * tileSize + 8.f));
        window.draw(heroShape);
        window.draw(title);
        window.draw(info);
        window.draw(sprite);

        window.display();
    }
}
