

#include <vjConfig.h>
#include <Environment/vjSyncWriter.h>

vjSyncWriter::vjSyncWriter(): ofstream(), mutex() {
    cout << "syncwriter was initialized" << endl;
}

int vjSyncWriter::lock() {
    return mutex.acquire();
}

int vjSyncWriter::unlock() {
    return mutex.release();
}
