#ifndef GameOverEnum_hpp
#define GameOverEnum_hpp
enum class GameOver { NO = 0, ALL_POINTS_COLLECTED, CATCHED_BY_GHOST };
#define gameOverToString(g) (g == GameOver::NO? "NO" : (g == GameOver::ALL_POINTS_COLLECTED? "ALL_POINTS_COLLECTED" : (g == GameOver::CATCHED_BY_GHOST? "CATCHED_BY_GHOST" : "")))
#define PLANE_WIDTH 30
#endif /* GameOverEnum_hpp */