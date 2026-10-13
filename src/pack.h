#ifndef PACK_H
#define PACK_H

#include <type_traits>

namespace pack_detail {
    template <typename... Args>
    constexpr int packSize = sizeof...(Args);


    // assert
    template <int QueryIndex, typename... Args>
    struct _packIndexToType {
        static_assert(QueryIndex >= 0 && QueryIndex < packSize<Args...>, "Pack index out of bounds");
    };

    // recursive
    template <int QueryIndex, typename First, typename... Rest>
    struct _packIndexToType<QueryIndex, First, Rest...> {
        using type = typename _packIndexToType<QueryIndex - 1, Rest...>::type;
    };

    // base
    template <typename First, typename... Rest>
    struct _packIndexToType<0, First, Rest...> {
        using type = First;
    };

    // wrapper
    template <int QueryIndex, typename... Args> using packIndexToType = typename _packIndexToType<QueryIndex, Args...>::type;


    // recursive
    template <int Accumulated, typename QueryType, typename First, typename... Rest>
    constexpr int _packTypeToIndex() {
        if(std::is_same_v<QueryType, First>) {
            return Accumulated;
        }

        return _packTypeToIndex<Accumulated + 1, QueryType, Rest...>();
    }

    // base
    template <int Accumulated, typename QueryType>
    constexpr int _packTypeToIndex() {
        return -1;
    }

    // wrapper
    template <typename QueryType, typename... Args>
    constexpr int packTypeToIndex = _packTypeToIndex<0, QueryType, Args...>();
}

template <typename... Args>
struct Pack {
    constexpr static int size = pack_detail::packSize<Args...>;
};

#endif  // PACK_H
