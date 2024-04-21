/*
 * Created by janis on 2024-02-26
 */

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <map>

template<typename T>
void print_vec(const std::vector<T>& vec) {
    std::cout << "[";
    if (vec.size() > 0) {
        std::cout << vec[0];
        for (size_t i = 1; i < vec.size(); i++) {
            std::cout << ", " << vec[i];
        }
    }
    std::cout << "]";
}

template<typename T>
void print_vec(const std::vector<std::pair<T, T>>& vec) {
    std::cout << "[";
    if (vec.size() > 0) {
        std::cout << std::endl << "  (" << vec[0].first << ", " << vec[0].second << ")";
        for (size_t i = 1; i < vec.size(); i++) {
            std::cout << ", " << std::endl << "  (" << vec[i].first << ", " << vec[i].second << ")";
        }
        std::cout << std::endl;
    }
    std::cout << "]";
}

template<typename A, typename B>
void print_map(const std::map<A, B>& map) {
    std::cout << "{";
    if (map.size() > 0) {
        auto iter = map.begin();
        std::cout << std::endl << "  " << iter->first << ": " << iter->second;
        iter++;
        for (; iter != map.end(); iter++) {
            std::cout << ", " << std::endl << "  " << iter->first << ": " << iter->second;
        }
        std::cout << std::endl;
    }
    std::cout << "}";
}

std::vector<std::pair<int64_t, int64_t>> myers_recursive(const std::string& a, const std::string& b, int64_t x, int64_t y, int64_t w, int64_t h) {
    const int64_t m = w;
    const int64_t n = h;
    const int64_t max = m + n;
    auto best_x_forward = std::vector<int64_t>(max * 2 + 1, -1);
    auto best_x_backward = std::vector<int64_t>(max * 2 + 1, -1);
    best_x_forward[max + 1] = 0;
    best_x_backward[max + 1] = 0;
    int64_t snake_start_x = -1;
    int64_t snake_start_y = -1;
    int64_t snake_end_x = -1;
    int64_t snake_end_y = -1;

    std::cout << "(x, y) = (" << x << ", " << y << ")    (w, h) = (" << w << ", " << h << ")" << std::endl;

    for (int64_t operations = 0; operations <= max; operations++) {
        for (int64_t diagonal = -(operations - 2 * std::max(0L, operations - n)); diagonal <= (operations - 2 * std::max(0L, operations - m)); diagonal += 2) {
            if (diagonal == -operations) {
                best_x_forward[max + diagonal] = best_x_forward[max + diagonal + 1];
            } else if (diagonal == operations) {
                best_x_forward[max + diagonal] = best_x_forward[max + diagonal - 1] + 1;
            } else if (best_x_forward[max + diagonal - 1] > best_x_forward[max + diagonal + 1]) {
                best_x_forward[max + diagonal] = best_x_forward[max + diagonal - 1] + 1;
            } else {
                best_x_forward[max + diagonal] = best_x_forward[max + diagonal + 1];
            }

            int64_t snake_start = best_x_forward[max + diagonal];
            while (best_x_forward[max + diagonal] < m &&
                best_x_forward[max + diagonal] - diagonal < n &&
                a[x + best_x_forward[max + diagonal]] == b[y + best_x_forward[max + diagonal] - diagonal]) {
                best_x_forward[max + diagonal]++;
            }

            std::cout << "  (" << operations << ", " << diagonal << ") best_x_forward = ";
            print_vec(best_x_forward);
            std::cout << std::endl;

            if ((-max < -diagonal + m - n) && (-diagonal + m - n < max) && (best_x_forward[max + diagonal] != -1) && (best_x_backward[max - diagonal + m - n] != -1)
                && (best_x_forward[max + diagonal] + best_x_backward[max - diagonal + m - n] >= m)) {
                std::cout << "    " << -max << " < " << -diagonal + m - n << " && " << -diagonal + m - n << " < " << max << std::endl;
                std::cout << "    diagonal = " << diagonal << ", -diagonal + m - n = " << -diagonal + m - n << std::endl;
                std::cout << "    " << best_x_forward[max + diagonal] << " + " << best_x_backward[max - diagonal + m - n] << " >= " << m << std::endl;

                snake_start_x = snake_start;
                snake_start_y = snake_start - diagonal;
                snake_end_x = best_x_forward[max + diagonal];
                snake_end_y = best_x_forward[max + diagonal] - diagonal;
                break;
            }
        }

        if (snake_start_x != -1) {
            break;
        }

        for (int64_t diagonal = -(operations - 2 * std::max(0L, operations - n)); diagonal <= (operations - 2 * std::max(0L, operations - m)); diagonal += 2) {
            if (diagonal == -operations) {
                best_x_backward[max + diagonal] = best_x_backward[max + diagonal + 1];
            } else if (diagonal == operations) {
                best_x_backward[max + diagonal] = best_x_backward[max + diagonal - 1] + 1;
            } else if (best_x_backward[max + diagonal - 1] > best_x_backward[max + diagonal + 1]) {
                best_x_backward[max + diagonal] = best_x_backward[max + diagonal - 1] + 1;
            } else {
                best_x_backward[max + diagonal] = best_x_backward[max + diagonal + 1];
            }

            int64_t snake_end = best_x_backward[max + diagonal];
            while (0 < m - best_x_backward[max + diagonal] &&
                0 < n - best_x_backward[max + diagonal] + diagonal &&
                a[x + m - best_x_backward[max + diagonal] - 1] == b[y + n - best_x_backward[max + diagonal] + diagonal - 1]) {
                best_x_backward[max + diagonal]++;
            }

            std::cout << "  (" << operations << ", " << diagonal << ") best_x_backward = ";
            print_vec(best_x_backward);
            std::cout << std::endl;

            if ((-max < -diagonal + m - n) && (-diagonal + m - n < max) && (best_x_forward[max + diagonal] != -1) && (best_x_backward[max - diagonal + m - n] != -1)
                && (best_x_forward[max - diagonal + m - n] + best_x_backward[max + diagonal] >= m)) {
                std::cout << "    " << -max << " < " << -diagonal + m - n << " && " << -diagonal + m - n << " < " << max << std::endl;
                std::cout << "    diagonal = " << diagonal << ", -diagonal + m - n = " << -diagonal + m - n << std::endl;
                std::cout << "    " << best_x_forward[max + diagonal] << " + " << best_x_backward[max - diagonal + m - n] << " >= " << m << std::endl;

                snake_start_x = m - best_x_backward[max + diagonal];
                snake_start_y = n - best_x_backward[max + diagonal] + diagonal;
                snake_end_x = m - snake_end;
                snake_end_y = n - snake_end + diagonal;
                break;
            }
        }

        if (snake_start_x != -1) {
            break;
        }
    }

    auto result = std::vector<std::pair<int64_t, int64_t>>();

    w = snake_start_x;
    h = snake_start_y;
    if (w == 0 || h == 0) {
        if (w != 0 || h != 0) {
            result.insert(result.end(), std::pair(w, h));
        }
    } else if (w == m && h == n) {
        if (w > h) {
            result.insert(result.end(), std::pair(h, h));
            result.insert(result.end(), std::pair(w - h, 0));
        } else {
            result.insert(result.end(), std::pair(w, w));
            result.insert(result.end(), std::pair(0, h - w));
        }
    } else {
        auto pre = myers_recursive(a, b, x, y, w, h);
        result.insert(result.end(), pre.begin(), pre.end());
    }

    if (snake_end_x - snake_start_x > 0) {
        result.insert(result.end(), std::pair(snake_end_x - snake_start_x, snake_end_x - snake_start_x));
    }

    w = m - snake_end_x;
    h = n - snake_end_y;
    if (w == 0 || h == 0) {
        if (w != 0 || h != 0) {
            result.insert(result.end(), std::pair(w, h));
        }
    } else if (w == m && h == n) {
        if (w > h) {
            result.insert(result.end(), std::pair(w - h, 0));
            result.insert(result.end(), std::pair(h, h));
        } else {
            result.insert(result.end(), std::pair(0, h - w));
            result.insert(result.end(), std::pair(w, w));
        }
    } else {
        auto post = myers_recursive(a, b, x + snake_end_x, y + snake_end_y, w, h);
        result.insert(result.end(), post.begin(), post.end());
    }

    return result;
}

std::vector<std::pair<int64_t, int64_t>> myers_diff(const std::string& a, const std::string& b) {
    std::vector<std::pair<int64_t, int64_t>> result = myers_recursive(a, b, 0, 0, a.size(), b.size());

    for (size_t i = 0; i < result.size() - 1; ++i) {
        if (result[i].first == 0 && result[i + 1].first == 0) {
            result[i] = std::pair(0, result[i].second + result[i + 1].second);
            result.erase(result.begin() + i + 1);
            i--;
        } else if (result[i].second == 0 && result[i + 1].second == 0) {
            result[i] = std::pair(result[i].first + result[i + 1].first, 0);
            result.erase(result.begin() + i + 1);
            i--;
        } else if (result[i].first == result[i].second && result[i + 1].first == result[i + 1].second) {
            result[i] = std::pair(result[i].first + result[i + 1].first, result[i].second + result[i + 1].second);
            result.erase(result.begin() + i + 1);
            i--;
        }
    }

    /*
    for (size_t i = 0; i < result.size() - 1; ++i) {
        if ((result[i].first == 0 && result[i + 1].second == 0) || (result[i + 1].first == 0 && result[i].second == 0)) {
            result[i] = std::pair(result[i].first + result[i + 1].first, result[i].second + result[i + 1].second);
            result.erase(result.begin() + i + 1);
            i--;
        }
    }
    */

    return result;
}

std::vector<std::pair<size_t, size_t>> patience_sort(const std::map<size_t, size_t>& index_matches) {
    struct PileElement {
        size_t a_index;
        size_t b_index;
        size_t prev;
    };

    std::vector<std::vector<PileElement>> piles;

    for (auto [a_index, b_index] : index_matches) {
        if (piles.size() == 0) {
            piles.push_back({PileElement(a_index, b_index, 0)});
            continue;
        }

        for (size_t i = 0; i <= piles.size(); i++) {
            if (i == piles.size()) {
                piles.push_back({PileElement(a_index, b_index, piles[i - 1].size() - 1)});
                break;
            }

            if (piles[i][piles[i].size() - 1].b_index > b_index) {
                piles[i].push_back(PileElement(a_index, b_index, piles[i - 1].size() - 1));
                break;
            }
        }
    }

    std::vector<std::pair<size_t, size_t>> increasing_matches;
    size_t prev_pile_element = piles[piles.size() - 1].size() - 1;

    for (size_t i = piles.size(); i > 0; i--) {
        PileElement e = piles[i - 1][prev_pile_element];
        increasing_matches.insert(increasing_matches.begin(), std::pair(e.a_index, e.b_index));
        prev_pile_element = e.prev;
    }

    return increasing_matches;
}

std::vector<std::pair<int64_t, int64_t>> patience_recursive(const std::string& a, const std::string& b, int64_t x, int64_t y, int64_t w, int64_t h) {
    std::map<char, size_t> a_count;
    std::map<char, size_t> a_index;
    std::map<char, size_t> b_count;
    std::map<char, size_t> b_index;

    for (int64_t i = 0; i < w; i++) {
        a_count[a[x + i]] += 1;
        a_index[a[x + i]] += i;
    }

    for (int64_t i = 0; i < h; i++) {
        b_count[b[y + i]] += 1;
        b_index[b[y + i]] += i;
    }

    std::map<size_t, size_t> index_matches;

    for (auto [element, count] : a_count) {
        if (count == 1 && b_count[element] == 1) {
            index_matches[a_index[element]] = b_index[element];
        }
    }

    if (index_matches.size() == 0) {
        return myers_recursive(a, b, x, y, w, h);
    }

    std::vector<std::pair<size_t, size_t>> increasing_matches = patience_sort(index_matches);

    std::vector<std::pair<int64_t, int64_t>> diff;

    size_t prev_a_index = 0;
    size_t prev_b_index = 0;

    for (auto [a_index, b_index] : increasing_matches) {
        size_t a_delta = a_index - prev_a_index;
        size_t b_delta = b_index - prev_b_index;

        if (a_delta > 0 && b_delta > 0) {
            std::vector<std::pair<int64_t, int64_t>> sub = patience_recursive(a, b, prev_a_index, prev_b_index, a_delta, b_delta);
            diff.insert(diff.end(), sub.begin(), sub.end());
        } else if (a_delta > 0) {
            diff.push_back(std::pair(a_delta, 0));
        } else if (b_delta > 0) {
            diff.push_back(std::pair(0, b_delta));
        }

        if (diff[diff.size() - 1].first == diff[diff.size() - 1].second) {
            diff[diff.size() - 1].first += 1;
            diff[diff.size() - 1].second += 1;
        } else {
            diff.push_back(std::pair(1, 1));
        }

        prev_a_index = a_index + 1;
        prev_b_index = b_index + 1;
    }

    size_t a_delta = w - prev_a_index;
    size_t b_delta = h - prev_b_index;

    if (a_delta > 0) {
        diff.push_back(std::pair(a_delta, 0));
    }

    if (b_delta > 0) {
        diff.push_back(std::pair(0, b_delta));
    }

    return diff;
}

std::vector<std::pair<int64_t, int64_t>> patience_diff(const std::string& a, const std::string& b) {
    std::vector<std::pair<int64_t, int64_t>> result = patience_recursive(a, b, 0, 0, a.size(), b.size());

    return result;
}

int main(void) {
    std::string a = "asdbhj";
    std::string b = "bsdfj";

    std::vector<std::pair<int64_t, int64_t>> result = patience_diff(a, b);
    std::cout << std::endl << "final result: ";
    print_vec(result);
    std::cout << std::endl;

    return 0;
}
