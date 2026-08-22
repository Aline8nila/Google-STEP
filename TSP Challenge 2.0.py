# Nearest Neighbor + 2-opt

import pandas as pd
from google.colab import files

uploaded = files.upload()

filename = list(uploaded.keys())[0]
cities_df = pd.read_csv(filename)
cities = list(zip(cities_df["x"], cities_df["y"]))


def score_tour(cities, tour):
    total = 0

    for i in range(len(tour)):
        current = tour[i]
        next_city = tour[(i + 1) % len(tour)]

        dx = cities[current][0] - cities[next_city][0]
        dy = cities[current][1] - cities[next_city][1]

        total += (dx * dx + dy * dy) ** 0.5

    return total


def solve_from_start(cities, start):
    n = len(cities)

    # Cities that haven't been visited yet
    unvisited = set(range(n))
    unvisited.remove(start)

    # Start with the chosen city
    tour = [start]
    current = start

    while unvisited:
        closest_city = None
        closest_distance = float("inf")

        for city in unvisited:
            dx = cities[current][0] - cities[city][0]
            dy = cities[current][1] - cities[city][1]

            # No square root needed for comparison
            distance = dx * dx + dy * dy

            if distance < closest_distance:
                closest_distance = distance
                closest_city = city

        tour.append(closest_city)
        unvisited.remove(closest_city)
        current = closest_city

    return tour


def two_opt(cities, tour):
    n = len(tour)
    improved = True

    while improved:
        improved = False

        for i in range(n - 2):
            for j in range(i + 2, n):
                a = tour[i]
                b = tour[i + 1]
                c = tour[j]
                d = tour[(j + 1) % n]

                old_distance = (((cities[a][0] - cities[b][0]) ** 2 + (cities[a][1] - cities[b][1]) ** 2) ** 0.5 + ((cities[c][0] - cities[d][0]) ** 2 + (cities[c][1] - cities[d][1]) ** 2) ** 0.5)

                new_distance = (((cities[a][0] - cities[c][0]) ** 2 + (cities[a][1] - cities[c][1]) ** 2) ** 0.5 + ((cities[b][0] - cities[d][0]) ** 2 + (cities[b][1] - cities[d][1]) ** 2) ** 0.5)

                if new_distance < old_distance:
                    tour[i + 1:j + 1] = reversed(tour[i + 1:j + 1])
                    improved = True

        print("2-opt pass done")

    return tour


def solve(cities):
    best_tour = None
    best_score = float("inf")

    # Try several starting cities
    num_starts = min(10, len(cities))

    for start in range(num_starts):
        tour = solve_from_start(cities, start)
        score = score_tour(cities, tour)
        if score < best_score:
            best_score = score
            best_tour = tour

    return best_tour


tour = solve(cities)
print("Before 2-opt:", round(score_tour(cities, tour), 2))

tour = two_opt(cities, tour)
print("After 2-opt:", round(score_tour(cities, tour), 2))


#Ant Colonie + 2-opt
