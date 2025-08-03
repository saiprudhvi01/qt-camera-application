#!/usr/bin/env python3
"""
Simple HTTP server for the Qt Camera Web Application
Serves the web interface locally with proper CORS headers for camera access
"""

import http.server
import socketserver
import webbrowser
import os
import sys
from pathlib import Path

class CameraHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    """Custom handler to add proper headers for camera access"""
    
    def end_headers(self):
        # Add headers to enable camera access
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        super().end_headers()
    
    def log_message(self, format, *args):
        """Override to customize log format"""
        print(f"[{self.log_date_time_string()}] {format % args}")

def main():
    # Configuration
    PORT = 8080
    HOST = 'localhost'
    
    # Change to the web directory
    web_dir = Path(__file__).parent
    os.chdir(web_dir)
    
    print("=" * 60)
    print("Qt Camera Web Application Server")
    print("=" * 60)
    print(f"Starting server on http://{HOST}:{PORT}")
    print(f"Serving files from: {web_dir.absolute()}")
    print()
    print("Features available:")
    print("✓ Live camera feed with HTML5 WebRTC")
    print("✓ Play, Pause, Resume, Stop controls")
    print("✓ Resolution selection (VGA, HD, Full HD)")
    print("✓ Camera device selection")
    print("✓ Screenshot capture and download")
    print("✓ Video recording (WebM format)")
    print("✓ Real-time frame rate monitoring")
    print("✓ Keyboard shortcuts (Arrow keys, Spacebar)")
    print()
    print("Keyboard Controls:")
    print("• Spacebar: Pause/Resume camera")
    print("• Left Arrow: Simulate rewind")
    print("• Right Arrow: Simulate forward")
    print()
    print("Note: Camera access requires user permission.")
    print("The browser will prompt you to allow camera access.")
    print()
    print("Press Ctrl+C to stop the server")
    print("=" * 60)
    
    try:
        # Create server
        with socketserver.TCPServer((HOST, PORT), CameraHTTPRequestHandler) as httpd:
            server_url = f"http://{HOST}:{PORT}"
            
            # Try to open browser automatically
            try:
                webbrowser.open(server_url)
                print(f"✓ Opened browser automatically at {server_url}")
            except Exception as e:
                print(f"Could not open browser automatically: {e}")
                print(f"Please open your browser and navigate to: {server_url}")
            
            print(f"\nServer running at {server_url}")
            print("Waiting for connections...")
            
            # Start serving
            httpd.serve_forever()
            
    except KeyboardInterrupt:
        print("\n\nShutting down server...")
        print("Thank you for using Qt Camera Web Application!")
    except OSError as e:
        if e.errno == 48:  # Address already in use
            print(f"\nError: Port {PORT} is already in use.")
            print("Please either:")
            print(f"1. Stop the other application using port {PORT}")
            print("2. Edit this script to use a different port")
        else:
            print(f"\nError starting server: {e}")
        sys.exit(1)
    except Exception as e:
        print(f"\nUnexpected error: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main() 