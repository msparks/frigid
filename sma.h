// Keeps an unweighted average of a fixed number of values.
template<typename T, int N>
class SimpleMovingAverage {
 public:
  SimpleMovingAverage() : head_(0), size_(0), average_(0) {}

  // Adds a new value, removing the oldest value if the array is
  // full. Returns the recomputed moving average.
  T valueNew(const T value) {
    if (size_ < N) {
      // The buffer is not yet full.

      // Insert new value at head and move head to next location in queue.
      values_[head_] = value;
      head_ = (head_ + 1) % N;
      ++size_;

      // Recompute average using new size.
      average_ = 0;
      for (int i = 0; i < size_; ++i) {
        average_ += values_[i];
      }
      average_ /= size_;
    } else {
      // Buffer is full. The size is fixed, so we do not need to recompute the
      // average over all values.

      // Avoid copying the old value; adjust the average first.
      average_ -= values_[head_] / size_;  // remove oldest value contribution
      average_ += value / size_;           // add new value contribution

      // Replace the oldest value (current head).
      values_[head_] = value;
      head_ = (head_ + 1) % N;
    }
  }

  // Returns the moving average value.
  T average() const {
    return average_;
  }

  // Returns the number of entries over which the average is computed.
  T size() const {
    return size_;
  }

 private:
  T values_[N];
  int head_;
  int size_;
  T average_;
};
