void help() {
    std::cout << "HFD cli tool" << std::endl
              << " - hdf-service-cli leds [color]"  << std::endl
              << " - hdf-service-cli vibrator [duration] [repeat]" << std::endl; 
}

int main(int argc, char** argv) {
    if(argc > 1) {
        
    } else 
        help();
}
