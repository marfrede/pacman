#ifndef GameOverEnum_hpp
#define GameOverEnum_hpp
enum class GameStatus { NOT_STARTED, ALL_POINTS_COLLECTED, CATCHED_BY_GHOST, PAUSE, PLAYING };
#define gameOverToString(g) (g == GameOver::NOT_STARTED? "NOT_STARTED" : (g == GameOver::ALL_POINTS_COLLECTED? "ALL_POINTS_COLLECTED" : (g == GameOver::CATCHED_BY_GHOST? "CATCHED_BY_GHOST" : (g == GameOver::PAUSE? "PAUSE" : (g == GameOver::PLAYING? "PLAYING" :)))
#define PLANE_WIDTH 30
#endif /* GameOverEnum_hpp */
