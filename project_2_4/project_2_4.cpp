#include <iostream>  // Gives us access to std::cin and std::cout
#include <string>  // Gives us access to std::string and std::getline()

struct LightOutputs {
  bool red_ns;
  bool yellow_ns;
  bool green_ns;
  bool red_ew;
  bool yellow_ew;
  bool green_ew;
};

struct LightInputs {
  int time;
  bool car_sensor_n;
  bool car_sensor_s;
};

// Define light controller state machine class here
class LightController {
private:
  enum State { // Declare enum values
    RED_ALL,
    YELLOW_NORTH_SOUTH,
    GREEN_NORTH_SOUTH,
    YELLOW_EAST_WEST,
    GREEN_EAST_WEST
  };

  State current_state_{RED_ALL};

  int next_transition_time_{0};

public:
  LightOutputs update(const LightInputs &inputs) {
    checkForTransitions(inputs);
    return runState();
  }

  void checkForTransitions(const LightInputs &inputs) {
    switch(current_state_) {
      case RED_ALL:
        if (inputs.car_sensor_n || inputs.car_sensor_s) {
          current_state_ = GREEN_NORTH_SOUTH;
        } else {
          current_state_ = GREEN_EAST_WEST;
        }
        next_transition_time_ = inputs.time + 5;
        break;
      case YELLOW_NORTH_SOUTH:
        if (inputs.time >= next_transition_time_) {
          current_state_ = RED_ALL;
        }
        break;
      case YELLOW_EAST_WEST:
        if (inputs.time >= next_transition_time_) {
          current_state_ = RED_ALL;
        }
        break;
      case GREEN_NORTH_SOUTH:
        if (inputs.time > next_transition_time_) {
          current_state_ = YELLOW_NORTH_SOUTH;
            next_transition_time_ = inputs.time + 2;
        }
        break;
      case GREEN_EAST_WEST:
        if (inputs.time > next_transition_time_) {
          current_state_ = YELLOW_EAST_WEST;
            next_transition_time_ = inputs.time + 2;
        }
        break;
    }
  }

  LightOutputs runState() {
      switch (current_state_) {
          case RED_ALL:
              return {true, false, false, true, false, false};
          case YELLOW_NORTH_SOUTH:
              return {false, true, false, true, false, false};
          case GREEN_NORTH_SOUTH:
              return {false, false, true, true, false, false};
          case YELLOW_EAST_WEST:
              return {true, false, false, false, true, false};
          case GREEN_EAST_WEST:
              return {true, false, false, false, false, true};
      }
  }


};


int main()
{
    // Initialize your state machine class here
  int time = 0;
  LightController controller;
  while(true)
  {
    std::string input;
    std::getline(std::cin, input);
    if(input.size() != 2)
        break;

    // Parse the input digits
    const LightInputs inputs {
      time,
      input[0] == '1',
      input[1] == '1'
    };

    // Update your state machine based on input
    const auto output = controller.update(inputs);

    // Output your state machine's light controls to std::cout
    std::cout << output.red_ns << output.yellow_ns << output.green_ns
                  << output.red_ew << output.yellow_ew << output.green_ew
                  << "\n";

  }
  return 0;
}
