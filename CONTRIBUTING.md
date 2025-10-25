# Contributing to ESP32-S3-Box3 Display Audio Photo

Thank you for your interest in contributing to this project! This document provides guidelines and instructions for contributing.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How Can I Contribute?](#how-can-i-contribute)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Submitting Changes](#submitting-changes)
- [Reporting Bugs](#reporting-bugs)
- [Suggesting Enhancements](#suggesting-enhancements)

## Code of Conduct

This project follows the [Contributor Covenant Code of Conduct](https://www.contributor-covenant.org/version/2/0/code_of_conduct/). By participating, you are expected to uphold this code.

## How Can I Contribute?

### Types of Contributions

We welcome contributions in the form of:

- 🐛 **Bug fixes**
- ✨ **New features**
- 📝 **Documentation improvements**
- 🧪 **Tests**
- 🎨 **UI/UX enhancements**
- ♻️ **Code refactoring**
- 🌐 **Translations**

### Good First Issues

Look for issues labeled `good first issue` if you're new to the project.

## Development Setup

### Prerequisites

1. **ESP-IDF v5.0 or later**
   ```bash
   . $HOME/esp/esp-idf/export.sh
   ```

2. **ESP32-S3-Box3 hardware** (for testing)

3. **Git**
   ```bash
   git --version
   ```

### Setting Up Your Development Environment

1. Fork the repository on GitHub

2. Clone your fork:
   ```bash
   git clone https://github.com/YOUR_USERNAME/esp-audio-video.git
   cd esp-audio-video
   ```

3. Add upstream remote:
   ```bash
   git remote add upstream https://github.com/DanielStuttgart/esp-audio-video.git
   ```

4. Create a branch for your changes:
   ```bash
   git checkout -b feature/my-awesome-feature
   ```

5. Build and test:
   ```bash
   idf.py build
   idf.py -p /dev/ttyUSB0 flash monitor
   ```

## Coding Standards

### C/C++ Code Style

We follow the ESP-IDF coding style:

- **Indentation:** 4 spaces (no tabs)
- **Line length:** Max 120 characters
- **Braces:** K&R style
  ```c
  if (condition) {
      // code
  } else {
      // code
  }
  ```
- **Naming:**
  - Functions: `snake_case`
  - Variables: `snake_case`
  - Constants: `UPPER_SNAKE_CASE`
  - Types: `snake_case_t`

### Code Example

```c
/**
 * @brief Brief description of function
 * 
 * Detailed description if needed.
 * 
 * @param param1 Description of param1
 * @param param2 Description of param2
 * @return Description of return value
 */
static esp_err_t my_function(int param1, const char *param2)
{
    if (param1 < 0) {
        ESP_LOGE(TAG, "Invalid parameter: %d", param1);
        return ESP_ERR_INVALID_ARG;
    }

    // Implementation
    ESP_LOGI(TAG, "Processing: %s", param2);
    
    return ESP_OK;
}
```

### Documentation

- **Every function must have a doc comment**
- **Use Doxygen-style comments**
- **Document parameters and return values**
- **Include usage examples for complex functions**

### Commit Messages

Write clear, descriptive commit messages:

```
Short summary (50 chars or less)

More detailed explanatory text, if necessary. Wrap it to about 72
characters or so. The blank line separating the summary from the body
is critical.

- Bullet points are okay
- Use present tense ("Add feature" not "Added feature")
- Use imperative mood ("Move cursor to..." not "Moves cursor to...")
```

**Good examples:**
- ✅ `Add volume control to Settings tab`
- ✅ `Fix JPEG decoder memory leak`
- ✅ `Update README with troubleshooting steps`

**Bad examples:**
- ❌ `Fixed stuff`
- ❌ `WIP`
- ❌ `asdf`

## Submitting Changes

### Pull Request Process

1. **Update your branch with latest upstream:**
   ```bash
   git fetch upstream
   git rebase upstream/main
   ```

2. **Test your changes:**
   - Build successfully: `idf.py build`
   - Flash to hardware: `idf.py flash monitor`
   - Test all affected features
   - Check for memory leaks
   - Verify no new warnings

3. **Update documentation:**
   - Update README.md if needed
   - Add inline code comments
   - Update CHANGELOG.md (if exists)

4. **Push to your fork:**
   ```bash
   git push origin feature/my-awesome-feature
   ```

5. **Create Pull Request:**
   - Go to GitHub and create a PR
   - Fill in the PR template
   - Link related issues
   - Add screenshots/videos if UI changed

6. **Address review feedback:**
   - Make requested changes
   - Push additional commits
   - Request re-review when ready

### Pull Request Checklist

Before submitting, ensure:

- [ ] Code builds without errors
- [ ] Code runs on actual hardware
- [ ] No new compiler warnings
- [ ] Code follows project style
- [ ] Documentation is updated
- [ ] Commit messages are clear
- [ ] No unnecessary files included
- [ ] PR description explains changes

## Reporting Bugs

### Before Submitting a Bug Report

- Check if the bug is already reported in [Issues](https://github.com/DanielStuttgart/esp-audio-video/issues)
- Try the latest version
- Verify it's not a hardware issue
- Collect relevant information

### Bug Report Template

```markdown
**Describe the bug**
A clear description of what the bug is.

**To Reproduce**
Steps to reproduce the behavior:
1. Go to '...'
2. Click on '...'
3. See error

**Expected behavior**
What you expected to happen.

**Actual behavior**
What actually happened.

**Screenshots**
If applicable, add screenshots.

**Environment:**
 - Board: ESP32-S3-Box3
 - ESP-IDF Version: [e.g., v5.1]
 - Project Version: [e.g., commit hash]
 - OS: [e.g., Ubuntu 22.04]

**Serial Monitor Output**
```
Paste relevant log output here
```

**Additional context**
Any other relevant information.
```

## Suggesting Enhancements

### Enhancement Template

```markdown
**Is your feature request related to a problem?**
A clear description of the problem.

**Describe the solution you'd like**
What you want to happen.

**Describe alternatives you've considered**
Other solutions you've thought about.

**Additional context**
Mockups, examples, links, etc.

**Implementation ideas**
If you have ideas about how to implement this.
```

## Development Tips

### Debugging

Enable verbose logging in `main/main.c`:
```c
esp_log_level_set("*", ESP_LOG_DEBUG);
```

### Memory Profiling

Check heap usage:
```c
ESP_LOGI(TAG, "Free heap: %d bytes", esp_get_free_heap_size());
```

### Performance Testing

Use LVGL performance monitor:
- Enabled in `sdkconfig.bsp.esp-box-3`
- Shows FPS on screen

### Common Issues

**Build fails with component not found:**
```bash
idf.py reconfigure
idf.py build
```

**SPIFFS won't mount:**
```bash
idf.py erase-flash
idf.py flash
```

## Questions?

- 💬 [GitHub Discussions](https://github.com/DanielStuttgart/esp-audio-video/discussions)
- 📧 Create an issue for questions
- 🌐 [ESP32 Forum](https://www.esp32.com/)

## License

By contributing, you agree that your contributions will be licensed under the same license as the project (Apache 2.0 / CC0-1.0).

---

Thank you for contributing! 🎉
