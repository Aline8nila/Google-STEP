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

            distance = dx * dx + dy * dy

            if distance < closest_distance:
                closest_distance = distance
                closest_city = city

        tour.append(closest_city)
        unvisited.remove(closest_city)
        current = closest_city

    return tour


def solve(cities):
    best_tour = None
    best_score = float("inf")

    # Try several different starting cities
    num_starts = min(10, len(cities))

    for start in range(num_starts):
        tour = solve_from_start(cities, start)
        score = score_tour(cities, tour)

        if score < best_score:
            best_score = score
            best_tour = tour

    return best_tour


tour = solve(cities)

print("Score:", round(score_tour(cities, tour), 2))
print("First 20 cities:", tour[:20])
