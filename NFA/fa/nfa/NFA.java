package fa.nfa;

import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.LinkedList;
import java.util.List;
import java.util.Set;
import fa.State;
import fa.dfa.DFA;

/**
 * Implementation of NFA class which contains functions to construct an
 * NFA from input and construct a corresponding DFA with getDFA().
 * @author Grayson
 */
public class NFA implements fa.FAInterface, fa.nfa.NFAInterface {

	private NFAState start;
	private Set<Character> abc;
	private Set<NFAState> states;

	/**
	 * Constructor for NFA class
	 */
	public NFA() {
		abc = new LinkedHashSet<Character>();
		states = new LinkedHashSet<NFAState>();
	}

	/**
	 * Adds a final state to working NFA
	 */
	public void addFinalState(String nextToken) {
		NFAState finalS = new NFAState(nextToken, true);
		states.add(finalS);
	}

	/**
	 * Adds a start state to working NFA
	 */
	public void addStartState(String startStateName) {
		start = getState(startStateName);
		if (start == null) {
			start = new NFAState(startStateName);
			states.add(start);
		}

	}

	/**
	 * @return start state
	 */
	private NFAState getStart() {
		return start;
	}

	/**
	 * Adds a state to working nfa
	 */
	public void addState(String nextToken) {
		NFAState state = new NFAState(nextToken);
		states.add(state);

	}

	/**
	 * Sets transition from one NFA state to another
	 */
	public void addTransition(String valueOf, char c, String valueOf2) {

		(getState(valueOf)).addTransition(c, getState(valueOf2));
		if (!abc.contains(c) && c != 'e') {
			abc.add(c);
		}

	}

	/**
	 * Returns the NFA state associated with a given string
	 * @param String name
	 * @return NFA state
	 */
	private NFAState getState(String name) {
		NFAState state = null;
		for (NFAState s : states) {
			if (s.getName().equals(name)) {
				state = s;
				break;
			}
		}
		return state;
	}

	/**
	 * Constructs a DFA from working NFA
	 */
	public DFA getDFA() {

		DFA dfa = new DFA();
		//instantiate a list for searching and a set of explored states
		List<Set<NFAState>> search = new LinkedList<>();
		Set<Set<NFAState>> explored = new LinkedHashSet<>();
		//instantiate a map of states to names
		HashMap<Set<NFAState>, String> dfastates = new HashMap<>();
		// add start state to search list
		Set<NFAState> start = eClosure(getStart());
		search.add(start);
		if (containsFinal(start)) {

			dfa.addFinalState(start.toString());
		}
		dfastates.put(start, start.toString());
		dfa.addStartState(start.toString());
		while (!search.isEmpty()) {
			Set<NFAState> set = search.remove(0);
			explored.add(set);

			//search for possible transitions
			for (char c : abc) {
				Set<NFAState> temp = new LinkedHashSet<>();
				for (NFAState nfas : set) {
					if (nfas.getTo(c) != null) {
						for (NFAState toState : nfas.getTo(c)) {
							temp.addAll(eClosure(toState));
						}
					}
				}
			//if toState is final make a final DFAState
				if (!explored.contains(temp)) {
					if (containsFinal(temp)) {
						dfastates.put(temp, temp.toString());
						dfa.addFinalState(dfastates.get(temp));
						explored.add(temp);
					}
			//if toState isn't final make a DFAState
					else {
						dfastates.put(temp, temp.toString());
						dfa.addState(dfastates.get(temp));
						explored.add(temp);
					}
			//Add new states to search
					if (!search.contains(temp))
						search.add(temp);
				}
			//Add transition to new state
				dfa.addTransition(dfastates.get(set), c, dfastates.get(temp));
			}
		}
		return dfa;

	}

	/**
	 * Iterates through a set of NFAStates looking for a final state
	 * @param Set of NFAStates
	 * @return boolean 
	 */
	private boolean containsFinal(Set<NFAState> set) {
		for (NFAState state : set) {
			if (state.isFinal()) {
				return true;
			}
		}
		return false;
	}

	@Override
	public Set<NFAState> getToState(NFAState from, char onSymb) {
		return from.getTo(onSymb);
	}

	@Override
	public Set<NFAState> eClosure(NFAState s) {

		List<NFAState> search = new LinkedList<>();
		Set<NFAState> explored = new LinkedHashSet<>();
		search.add(s);
		explored.add(s);

		while (!search.isEmpty()) {
			NFAState n = search.remove(0);
			explored.add(n);
			if (n.getTo('e') == null) {
				return explored;
			}
			for (NFAState states : n.getTo('e'))
				if (!explored.contains(states)) {
					search.add(states);
				}

		}

		return explored;
	}

	@Override
	public Set<State> getStates() {
		Set<State> ret = new LinkedHashSet<State>();
		ret.addAll(states);
		return ret;
	}

	@Override
	public Set<State> getFinalStates() {
		Set<State> ret = new LinkedHashSet<State>();
		for (NFAState s : states) {
			if (s.isFinal()) {
				ret.add(s);
			}
		}
		return ret;
	}

	@Override
	public State getStartState() {
		return start;
	}

	@Override
	public Set<Character> getABC() {
		return abc;
	}

}
