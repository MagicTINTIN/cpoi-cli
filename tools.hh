void help(std::string binary);
void helpError(std::string binary);
int arguments(std::vector<std::string> &args, std::string const &v, std::string &mode, std::string &value);
std::string urlEncode(const std::string &input);

void setMode(std::string &mode);
void setValue(std::string &value);
void setCode(std::string &code);

void sendRequest(std::string &instance, std::string &mode, std::string &value);
std::string request(std::string &instance, std::string const &mode, std::string const &value);