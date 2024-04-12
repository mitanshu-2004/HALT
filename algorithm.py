
def adjust_traffic_lights(cars_in_lane):
    min_green_time = 10  # Minimum green light time in seconds
    max_green_time = 60  # Maximum green light time in seconds

    scaling_factor = 1 + (cars_in_lane / 15)  # Adjust as needed based on traffic patterns
    
    # Adjust green light time based on the scaling factor
    green_time = min(max(min_green_time, scaling_factor * min_green_time), max_green_time)

    return green_time;
