# AES | Rijndael algorithm

## This rijndael implementation have next features:
- easy to learn
- easy to use

## Future changes:
- 192 bit implementation
- 256 bit implementation
- build-in password hashing

## Usage
##### Executing functuions:
``` cpp
aes128 aes;

const std::string original_message = "hello world";
const auto encrypted_message = aes.encrypt(original_message, "goodby");
const auto decrypted_message = aes.decrypt(encrypted_message, "goodby");
```

## Contributing
Pull requests are welcome. For major changes, please open an issue first to discuss what you would like to change.

## License
[MIT](https://choosealicense.com/licenses/mit/)
