import murmurhash.mrmr
import uuid


def test_hashes():
    assert murmurhash.mrmr.hash("hello world") == 1586663183
    murmurhash.mrmr.hash("anxiety") == -1859125401

for _ in range(10 * 1000 * 1000):
    print(murmurhash.mrmr.hash(uuid.uuid4().bytes))
