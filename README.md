# LinkedList

CS-300

Code Reflection

	In developing this code, I found it simpler and more straightforward to use a dummy node in the construction of the linked list. Use of a dummy node
  saves lines of code by ensuring that neither the head nor the tail ever points to nullptr. Implementing dummy nodes simplifies all the code of the
  append, prepend, search, remove, and insert operations since it allows us to always have every node pointed to by, and pointing to another node. 
One issue I ran into was having to fix a few things in the switch-case statement in main(). There was a missing cin statement to get the input for the
argument to be passed to the Search function. Added some logic to prevent a duplicate bidIds from being entered through case 1 (append function). I added a
boolean variable flag to make sure the bids won’t be loaded more than once from the CSV file, displaying a little explanation message instead of the clock
time. I also updated case 9(exit)’s message to feel a little more user friendly than a simple “Good bye”. Overall the assignment and this week’s textbook
has helped me with linked lists in general.
