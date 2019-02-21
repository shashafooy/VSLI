Aaron Pettit
A02107849

InitialRouting() - Performs the initial routing of the given benchmark file. This routing is basic with potential congestion and improvements can be made.

RRR() - Rip up and Reroute will try to fix the congestion cause by the Initial Routing() step. If there is congestion, the algorithm redo the routing for the wires involved in the congestion.

GreedyImprovements() - last step in optimizing the Routing. This algorithm will attempt to reduce the wire length of all existing nets while not causing congestion.


SimpleGR adds basic commands such as addSegment and ripUpNet. These functions are repeatable code where a function can be used to do an operation on the given net and edge. Making these generic commands gives a basic flow and makes it easy to route a single net. This also helps with self documenting code, when a function is called addSegment(), it describes what it does in the name.