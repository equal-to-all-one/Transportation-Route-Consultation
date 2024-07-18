import heapq
import datetime

class TransportMode:
    NONE = "none"
    AIRPLANE = "airplane"
    HIGH_SPEED_RAIL = "high_speed_rail"

class Route:
    def __init__(self, start_city, destination_city, start_time, arrival_time, time_cost, money_cost, distance, mode):
        self.start_city = start_city
        self.destination_city = destination_city
        self.start_time = start_time
        self.arrival_time = arrival_time
        self.time_cost = time_cost
        self.money_cost = money_cost
        self.distance = distance
        self.mode = mode

class City:
    def __init__(self, name, longitude, altitude):
        self.name = name
        self.longitude = longitude
        self.altitude = altitude
        self.route_table = []

class RouteGraph:
    def __init__(self):
        self.adjoin_graph = []

    def parse_time(self, time_str):
        return datetime.datetime.strptime(time_str, "%Y-%m-%d %H:%M")

    def calculate_transfer_time(self, arrival_time_str, start_time_str):
        arrival_time = self.parse_time(arrival_time_str)
        start_time = self.parse_time(start_time_str)
        transfer_duration = (start_time - arrival_time).total_seconds()
        if transfer_duration < 0:
            return -1, -1
        hours = transfer_duration // 3600
        minutes = (transfer_duration % 3600) // 60
        return int(hours), int(minutes)

    def calculate_fit_distance(self, origin, destination, mode, user_start_time):
        distances = {city.name: float('inf') for city in self.adjoin_graph}
        previous_city = {}
        previous_arrival_time = {}
        route_times = {}

        pq = []
        in_queue = set()
        processed = set()

        start_city = next((city for city in self.adjoin_graph if city.name == origin), None)
        end_city = next((city for city in self.adjoin_graph if city.name == destination), None)

        if not start_city:
            print("起点城市不存在")
            return
        elif not end_city:
            print("终点城市不存在")
            return

        distances[origin] = 0
        heapq.heappush(pq, (0, origin))
        in_queue.add(origin)
        previous_arrival_time[origin] = user_start_time

        while pq:
            current_distance, current_city = heapq.heappop(pq)
            in_queue.remove(current_city)
            processed.add(current_city)

            if current_city == destination:
                break

            city_iter = next((city for city in self.adjoin_graph if city.name == current_city), None)

            if city_iter:
                for route_list in city_iter.route_table:
                    best_route = route_list[0]
                    found_valid_route = False

                    for route in route_list:
                        if route.mode != mode:
                            continue

                        transfer_time = self.calculate_transfer_time(previous_arrival_time[current_city], route.start_time)
                        if transfer_time[0] < 0 or transfer_time[1] < 0:
                            continue

                        if route.destination_city in processed:
                            continue

                        if route.distance < best_route.distance or not found_valid_route:
                            best_route = route
                            found_valid_route = True

                    if found_valid_route:
                        new_distance = distances[current_city] + best_route.distance
                        if new_distance < distances[best_route.destination_city]:
                            distances[best_route.destination_city] = new_distance
                            previous_city[best_route.destination_city] = current_city
                            previous_arrival_time[best_route.destination_city] = best_route.arrival_time
                            route_times[best_route.destination_city] = (best_route.start_time, best_route.arrival_time)

                            if best_route.destination_city not in in_queue:
                                heapq.heappush(pq, (new_distance, best_route.destination_city))
                                in_queue.add(best_route.destination_city)

        if distances[destination] == float('inf'):
            print(f"no route from {origin} to {destination}.")
        else:
            print(f"the shortest distance from {origin} to {destination} is {distances[destination]}.")
            path = []
            city = destination
            while city != origin:
                path.append(city)
                city = previous_city[city]
            path.append(origin)
            path.reverse()
            print("Path: ", " -> ".join(path))
            print("Route details:")
            print(f"user start time: {user_start_time}")
            for i in range(1, len(path)):
                from_city = path[i-1]
                to_city = path[i]
                print(f"From {from_city} to {to_city} | Start: {route_times[to_city][0]} | Arrival: {route_times[to_city][1]}")

# Example Usage
# You need to create city instances and routes, then add them to the graph, then call the calculate_fit_distance function.