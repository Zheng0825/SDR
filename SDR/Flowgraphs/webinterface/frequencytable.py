
class FrequencyTable:

    freqs = [900000000, 915000000, 920000000, 2200000000, 2400000000, 2500000000, 5725000000]
    current_freq
    current_index

    def __init__(self):
        with self._lock:
            current_index = 0
            self.current_freq = freqs[current_index]
        

    def increase_freq(self):
        with self._lock:
            if current_index < length(freqs):
                current_index += 1
                current_freq = freqs[current_index]
            else:
                pass


    def decrease_freq(self):
        with self._lock:
            if current_index > 0:
                current_index -= 1
                current_freq = freqs[current_index]
            else:
                pass
