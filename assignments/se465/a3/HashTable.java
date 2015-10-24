/** hash tables using linear open addressing and division */
import java.util.*;

public class HashTable {
    protected static class HashEntry {
        protected Object key;
        protected Object element;

        private HashEntry() {}

        private HashEntry(Object theKey, Object theElement) {
            key = theKey;
            element = theElement;
        }
    }

    protected int divisor;
    protected HashEntry[] table;
    protected int size;

    public HashTable(int theDivisor) {
        divisor = theDivisor;

        table = new HashEntry[divisor];
    }

    /**
     * @return true iff the table is empty
     */
    public boolean isEmpty() {
        return size == 0;
    }

    /**
     * @return current number of elements in the table
     */
    public int size() {
        return size;
    }

    /**
     * search an open addressed hash table for an element with key theKey
     *
     * @return location of matching element if found, otherwise return
     * location where an element with key theKey may be inserted
     * provided the hash table is not full
     */
    private int search(Object theKey) {
        int i = Math.abs(theKey.hashCode()) % divisor;
        int j = i;

        do {
            if (table[j] == null || table[j].key.equals(theKey)) {
                return j;
            }

            j = (j + 1) % divisor;
        } while (j != i);

        return j;
    }

    /**
     * @return element with specified key
     * @return null if no matching element
     */
    public Object get(Object theKey)
    {
        int b = search(theKey);

        if (table[b] == null || !table[b].key.equals(theKey)) {
            return null;
        }

        return table[b].element;
    }

    /**
     * insert an element with the specified key
     * overwrite old element if there is already an
     * element with the given key
     *
     * @throws IllegalArgumentException when the table is full
     * @return old element (if any) with key theKey
     */
    public Object put(Object theKey, Object theElement)
    {
        int b = search(theKey);

        if (table[b] == null) {
            table[b] = new HashEntry(theKey, theElement);
            size++;

            return null;
        } else {
            //****    Duplicate element should be updated, ERROR ***
            if (table[b].key.equals(theKey)) {
                Object elementToReturn = table[b].element;

                return elementToReturn;
            } else {
                throw new IllegalArgumentException("table is full");
            }
        }
    }

    /**
     * output the hash table
     */
    public void output()
    {
        for (int i = 0; i < divisor; i++) {
            if (table[i] == null) {
                System.out.println("null");
            } else {
                System.out.println(table[i].key + "\t" + table[i].element);
            }
        }
    }

    public static void main (String [] args) {
        // HashTable h = new HashTable(11);
        // h.put(new Integer(80), new Integer(80));
        // h.put(new Integer(40), new Integer(40));
        // h.put(new Integer(65), new Integer(65));
        // h.output();
        // System.out.println();
        // h.put(new Integer(58), new Integer(58));
        // h.put(new Integer(24), new Integer(24));
        // h.output();
        // System.out.println();
        // h.put(new Integer(2), new Integer(2));
        // h.put(new Integer(13), new Integer(13));
        // h.put(new Integer(46), new Integer(46));
        // h.put(new Integer(16), new Integer(16));
        // h.put(new Integer(7), new Integer(7));
        // h.put(new Integer(21), new Integer(21));
        // h.output();
        // System.out.println();
        // try {h.put(new Integer(99), new Integer(99));}
        // catch (Exception e)
        // {System.out.println(" No memory for 99");}
        // System.out.println();
        // // update element
        // h.put(new Integer(7), new Integer(29));
        // h.output();

        HashTable table = new HashTable(1);
        table.put(new Integer(3), new Integer(7));
        table.put(new Integer(3), new Integer(42));
        table.output();
    }
}
