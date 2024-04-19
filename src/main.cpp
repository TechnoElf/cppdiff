/*
 * Created by janis on 2024-02-26
 */

#include <cstdint>
#include <vector>
#include <string>
#include <iostream>
#include <map>

void print_vec(const std::vector<int64_t>& vec) {
    std::cout << "[";
    if (vec.size() > 0) {
        std::cout << vec[0];
        for (size_t i = 1; i < vec.size(); i++) {
            std::cout << ", " << vec[i];
        }
    }
    std::cout << "]";
}

void print_vec(const std::vector<std::pair<int64_t, int64_t>>& vec) {
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

std::vector<std::pair<int64_t, int64_t>> histogram_recursive(const std::string& a, const std::string& b, size_t x, size_t y, size_t w, size_t h) {
    size_t equivalent_pre = 0;
    for (; w > 0 && h > 0 && a[x] == b[y]; equivalent_pre++) {
        x++;
        y++;
        w--;
        h--;
    }

    size_t equivalent_post = 0;
    for (; w > 0 && h > 0 && a[w - 1] == b[h - 1]; equivalent_post++) {
        w--;
        h--;
    }

    std::map<char, std::vector<size_t>> a_histogram;
    for (size_t line = 0; line < w; line++) {
        if (a_histogram.contains(a[line])) {
            auto chain = a_histogram[a[line]];
            chain.insert(chain.end(), line);
        } else {
            auto chain = std::vector<size_t>();
            chain.insert(chain.end(), line);
            a_histogram.insert_or_assign(a[line], chain);
        }
    }

    std::map<char, std::vector<size_t>> b_histogram;
    for (size_t line = 0; line < w; line++) {
        if (a_histogram.contains(b[line])) {
            if (b_histogram.contains(b[line])) {
                auto chain = b_histogram[b[line]];
                chain.insert(chain.end(), line);
            } else {
                auto chain = std::vector<size_t>();
                chain.insert(chain.end(), line);
                b_histogram.insert_or_assign(b[line], chain);
            }
        }
    }

    std::vector<std::pair<int64_t, int64_t>> result;

    if (equivalent_pre > 0) {
        result.insert(result.end(), std::pair(equivalent_pre, equivalent_pre));
    }

    if (b_histogram.size() == 0) {
        result.insert(result.end(), std::pair(w, 0));
        result.insert(result.end(), std::pair(0, h));
    } else {
        std::vector<std::pair<size_t, size_t>> pairs;
    }

    if (equivalent_post > 0) {
        result.insert(result.end(), std::pair(equivalent_post, equivalent_post));
    }

    return result;
}

std::vector<std::pair<int64_t, int64_t>> histogram_diff(const std::string& a, const std::string& b) {
    std::vector<std::pair<int64_t, int64_t>> result = histogram_recursive(a, b, 0, 0, a.size(), b.size());

    return result;
}

int main(void) {
    std::string a = "ksnv";
    std::string b = "s";

    std::vector<std::pair<int64_t, int64_t>> result = myers_diff(a, b);
    std::cout << std::endl << "final result: ";
    print_vec(result);
    std::cout << std::endl;

    return 0;
}
