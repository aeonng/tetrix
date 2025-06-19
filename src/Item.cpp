#include "Item.hpp"
#include "Utility.hpp"
#include "Board.hpp"
#include <random>
#include <algorithm>

int total_grey_cleared = 0;

static std::mt19937 rng(std::random_device{}());  // 고정 시드 X
static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

Item::Item(int t, int x, int y)
    : type(t), x(x), y(y) {}


int Item::applyEffect(const std::vector<int>& items, Board& board) {
    total_grey_cleared = 0;
    for (int type = items.size() - 1; type >= 0; --type) {
        for (int i = 0; i < items[type]; ++i) {
            applyEffect(type, board); // 기존 단건 함수 재사용
        }
    }
    return total_grey_cleared;
}

void Item::applyEffect(int type, Board& board) {
    switch (type) {
        case 0:
            total_grey_cleared += board.clearBottomLines(1);
            break;
        case 1:
            total_grey_cleared += board.clearBottomLines(2);
            break;
        case 2:
            board.addGreyRows(2);
            break;
        case 3:
            break; //dual mode 미구현
        default:
            break;
    }
}

int Item::getX() const { return x; }
int Item::getY() const { return y; }
int Item::getTypeId() const { return static_cast<int>(type); }

int Item::chooseItemType(float elapsedTime, bool isSingle) {
    static std::mt19937 rng(std::random_device{}());
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    constexpr float itemAppearChance = 0.2f;  // 고정 확률 (30%)

    float t = elapsedTime;
    float decay = std::min(t / 60.0f, 5.0f);  // 시간에 따라 item 내 분포만 변화

    float p_clear1 = 0.4f;
    float p_clear2 = 0.3f;
    float p_add    = isSingle ? 0.40f + decay * 0.03f : 0.25f + decay * 0.02f;
    float p_oppo   = isSingle ? 0.0f : 0.35f + decay * 0.01f;

    float total = p_clear1 + p_clear2 + p_add + p_oppo;
    p_clear1 /= total;
    p_clear2 /= total;
    p_add    /= total;
    p_oppo   /= total;

    float r = dist(rng);

    // 30% 확률로만 아이템 등장
    if (r > itemAppearChance) return -1;

    r = dist(rng);  // 다시 뽑아서 분포 선택
    if (r < p_clear1) return 0;
    else if (r < p_clear1 + p_clear2) return 1;
    else if (r < p_clear1 + p_clear2 + p_add) return 2;
    else return 3;
}