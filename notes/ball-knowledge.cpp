/* # File handling in C++ #
 *
 * In C++, file streams are used to read, write, or perform other operations
 * in a file. They are like the standard input, standard output, standard
 * error streams. The insertion operator (<<) and the extraction operator (>>)
 * both work for file streams.
 *
 * Streams are an interface that provide the functionality to read, write and
 * perform other operations on a file. Underneath, there is a read head and a
 * write head. They may be separate in other types of streams, but for file
 * streams, they are at the same position always.
 *
 * The fstream library is the combination of ifstream and ofstream (input file
 * stream and output file stream). 
 *
 * A file can be opened as follows:
 *
 * std::fstream file_name(<file path>, <mode>);
 *
 * The destructor automatically closes the file once it goes out of scope. This
 * is an improvement over C-style file handling using FILE pointers.
 *
 * available <mode> are:
 * - std::ios::in  --> default for std::ifstream
 * - std::ios::out --> default for std::ofstream
 * - std::ios::app --> append mode. Before writing, the read/write head is always
 *                     moved to the end
 * - std::ios::ate --> at the end mode. this puts the read/write head to the end
 *                     of the file but allows seek. It must be combined with
 *                     std::ios::in before any seekg() / seekp() + write combination,
 *                     otherwise the file contents will be truncated / lost.
 * - std::ios::binary --> opens the file in binary mode
 *
 * # Usefule operations #
 *
 * tellg() and tellp() return the absolute position of the read head (get head)
 * and write head (put head) respectively.
 *
 * seekg() and seekp() operations move the get head and put head respectively.
 * Both of these member functions take two arguments: the offset to move, and
 * the place to move from, which can be one of three things:
 *
 * std::ios::beg --> beginning of file
 * std::ios::cur --> current position of the read/write head
 * std::ios::end --> end of file
 *
 * The offset can be positive or negative. Positive moves towards the end of
 * file, negative moves towards the beginning.
 *
 * In case of file streams, since both heads are at the same position at all
 * times, so, either Xp or Xg functions are one and the same. Like, tellg()
 * and tellp() do the same thing, seekg() and seekp() do the same thing, etc.
 *
 * get() function returns a single byte from the file/stream.
 *
 * read() function reads the contents of the file. It takes two arguments.
 * First: the buffer in which to read into
 * Second: the number of bytes to read
 */

