# Multi-robot-simulation
Simulation experiments for multi-robots and global topological maps, where the global path is generated by the topological map(https://github.com/WILL-ZHAO-1/Topological_map-for-multirobot.git).

1. This simulation experiment rely on the simulation function package of Turtlebot3, please make sure it has been installed successfully.
2. Turtlebot_3gazebo and turtlebot3_navigation are function packages of external configuration files. Robot simulation is realized through Launch files, and the map environment is configured in Map files;
3. node-send provides the function of global path node sending. On the basis of node generation, you need to configure the original coordinates of the map, the generated node list, and the node matching list (all can be obtained in NODE GENERATION), and run node send through rosrun command.
4. check-map provides the mapping function from nodes to map coordinates. You need to provide node list information in the configuration file, and run the Check map node through rosrun command.
5. Show_point provides the visualization function of the node in the map, and publishes the final node information on the basis of the node list，Run the show node through the rosrun command.

