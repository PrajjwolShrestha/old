#include "Game.h"
#include "Player.h"
#include "Tilemap.h"
#include "Tail_node.h"

void Snake::Game::init() {
	if (SDL_Init(SDL_INIT_EVERYTHING) >= 0) {
		window = SDL_CreateWindow("SDL Snake",
			SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED,
			640, 480,
			SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
		if (window != 0) {
			renderer = SDL_CreateRenderer(window, -1,
				SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		}
	}
	else {
		Snake::Utils::logSDLError(std::cout, "Failed initializing SDL");
		is_running = false;
	}

	Tilemap* tilemap = Tilemap::instance();
	tilemap->init(renderer, 32, 32);
	tilemap->addTile("assets/grass.png", "grass");
	tilemap->addTile("assets/snake_head.png", "snake_head");
	tilemap->addTile("assets/snake_body.png", "snake_body");

	player = new Player();
	player->load(100, 100);
	for (int i = 0; i < 100; i++) {
		player->growTail();
	}

	is_running = true;
}

void Snake::Game::render() {
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	Tilemap::instance()->fillWith("grass", 0, 0, screen_w, screen_h);
	player->render();
	SDL_RenderPresent(renderer);
}

void Snake::Game::update() {
	player->update();
}

void Snake::Game::handleEvents() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			is_running = false;
			break;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_UP:
				player->setDirection(up);
				break;
			case SDLK_DOWN:
				player->setDirection(down);
				break;
			case SDLK_LEFT:
				player->setDirection(left);
				break;
			case SDLK_RIGHT:
				player->setDirection(right);
				break;
			case SDLK_ESCAPE:
				is_running = false;
				break;
			}
			break;
		case SDL_WINDOWEVENT:
			switch (event.window.event) {
			case SDL_WINDOWEVENT_SHOWN:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				SDL_GetWindowSize(window, &screen_w, &screen_h);
				break;
			}
			break;
		default:
			break;
		}
	}
}

void Snake::Game::clean() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
}