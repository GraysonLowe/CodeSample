package fa.nfa;

import java.util.HashMap;
import java.util.LinkedHashSet;
import java.util.Set;

import fa.State;

/**
 * Implementation of NFAState 
 * @author Grayson
 */
public class NFAState extends State{

	private HashMap<Character,Set<NFAState>> delta;
	private boolean isFinal;

	/**
	 * Constructor for NFAState
	 * @param name-String for name of NFAState
	 */
	public NFAState(String name){
		initDefault(name);
		isFinal = false;
	}
	/**
	 * Constructor for final NFAState
	 * @param name-name of NFAState
	 * @param isFinal-boolean for whether NFAState is final
	 */
	public NFAState(String name, boolean isFinal){
		initDefault(name);
		this.isFinal = isFinal;
	}
	/**
	 * Sets name and instantiates delta for NFAState
	 * @param name-String name of NFAState
	 */
	private void initDefault(String name ){
		this.name = name;
		delta = new HashMap<Character, Set<NFAState>>();
	}
	/**
	 * @return isFinal-boolean for whether state is final or not
	 */
	public boolean isFinal(){
		return isFinal;
	}

	/**
	 * Adds a transition to another NFAState
	 * @param onSymb-character of transition
	 * @param toState-state to transition to
	 */
	public void addTransition(char onSymb, NFAState toState){
		if(delta.get(onSymb)==null){
			Set<NFAState> trans = new LinkedHashSet<NFAState>();
			trans.add(toState);
			delta.put(onSymb, trans);

		}
		delta.get(onSymb).add(toState) ;
	}
	/**
	 * @param symb - character of transitions
	 * @return all possible transitions on given symbol
	 */
	public Set<NFAState> getTo(char symb){
		return delta.get(symb);
	}
}
