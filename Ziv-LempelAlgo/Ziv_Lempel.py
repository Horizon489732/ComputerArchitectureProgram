class Ziv_Lempel:
    def __init__(self, org_str):
        if not org_str:
            raise Exception("empty string")

        self.org_string = org_str[:]
        self.unique_substrings = []
        self.converted_substrings = []
        self.encoded_message = ""

    # Extract all unique substrings
    def get_unique_substrings(self, org_str=None):
        if org_str is None:
            org_string = self.org_string
        else:
            org_string = org_str
        
        unique_substrings_list = []
        substring_set = set()
        curr_str = ""

        for c in org_string:
            curr_str += c
            if curr_str in substring_set:
                continue #if existed, ignore and add the next char
            else:
                substring_set.add(curr_str)
                unique_substrings_list.append(curr_str)
                curr_str="" #reseting when we found a new substring

        if org_str is None:
            self.unique_substrings = unique_substrings_list

        return unique_substrings_list
    
    # Convert substrings by ignoring the last bit and convert index
    def translate_substrings(self, unique_substr = None):
        
        if unique_substr is None:
            unique_substrings = self.unique_substrings
        else:
            unique_substrings = unique_substr
        
        # Getting the map of substring to index
        index_map = {}
        for index, sub_str in enumerate(unique_substrings):
            index_map[sub_str] = index+1

        converted_substrings = []

        for sub_string in unique_substrings:
            if sub_string == "1":
                converted_substrings.append("1")
                continue  # Skip 0
            if sub_string == "0":
                converted_substrings.append("0")
                continue  # Skip 1

            # Ignoring the last character
            prefix = sub_string[:-1]
            last_bit = sub_string[-1]

            if prefix in index_map:
                # Get the index of the prefix
                index = index_map[prefix]
                converted_substrings.append(f"{index}{last_bit}")
        
        if unique_substr is None:
            self.converted_substrings = converted_substrings

        return converted_substrings
    
     # Convert the prefix of the converted substring to binary
    def get_encoded_message(self, converted_substr = None):
        if converted_substr is None:
            converted_substrings = self.converted_substrings
        
        # Calculating the maximum number of bits
        n = len(self.unique_substrings)
        num_bits = 1
        temp = n
        while temp > 1:
            temp //= 2
            num_bits += 1
        
        # If n is not not exactly a power of 2
        if 2 ** (num_bits - 1) < n:
            pass
        else:
            num_bits -= 1

        message = ""
        for sub_string in converted_substrings:
            if sub_string == "1" or sub_string == "0":
                message += sub_string
            else:
                last_bit = sub_string[-1]
                index_str = sub_string[:-1]
                binary_index = bin(int(index_str))[2:]
                binary_index = "0" * ((num_bits - 1) - len(binary_index)) + binary_index
                message += binary_index + last_bit

        if converted_substr is None:
            self.encoded_message = message
        
        return message

if __name__ == "__main__":
    z = Zen_Lempel("1010001101010110101")
    print(z.get_unique_substrings())
    print(z.translate_substrings())
    print(z.get_encoded_message())
    print(z.get_encoded_message() == "100010010000110101110010100111")