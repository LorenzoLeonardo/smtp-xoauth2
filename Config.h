#pragma once
#include <string>
#include <vector>

class Singleton {
  public:
    // Static member function to access the singleton instance
    static Singleton &getInstance() {
        static Singleton instance; // Initialized on first use.
        return instance;
    }

    // Delete the copy constructor and assignment operator
    Singleton(const Singleton &) = delete;
    Singleton &operator=(const Singleton &) = delete;

    // Other member functions and variables can be added as needed

  private:
    // Private constructor to prevent instantiation
    Singleton() {
        // Initialization code goes here
    }

    // Destructor to perform cleanup if needed
    ~Singleton() {
        // Cleanup code goes here
    }

    // Private data members can be added as needed
};

class Config {
  public:
    static class Config &getInstance() {
        static Config instance; // Initialized on first use.
        return instance;
    }

    struct Provider {
        std::string name;
        std::string profile_endpoint;
        std::string auth_endpoint;
        std::string device_auth_endpoint;
        std::string token_endpoint;
        std::string contacts_endpoint;
        std::string client_id;
        std::vector<std::string> scopes;
    };
    std::string server;
    unsigned short port;
    Provider provider;

    void loadConfigFromFile(const char *path);

    Config() { loadConfigFromFile("config.json"); }

    ~Config() {}
};
