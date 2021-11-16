/**
 * @brief Utility function to dictate whether two arrays contain equal contents.
 * 
 * @param array1 The first array.
 * @param array2 The second array.
 * @param length The length to compare of the two arrays.
 * @return True if arrays are equal.
 */
template<typename t1, typename t2>
bool arrayEqual(t1 *array1, t2 *array2, int length)
{
    for (int i = 0; i < length; i++)
    {
        if (array1[i] != array2[i]) return false;
    }
    return true;
    Serial.println("Equal");
}
