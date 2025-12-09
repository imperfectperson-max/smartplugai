# Contributing to Smart Plug AI

Thank you for your interest in contributing to Smart Plug AI! This document provides guidelines and instructions for contributing.

## 🌟 How to Contribute

### Reporting Bugs

Use the [bug report template](.github/ISSUE_TEMPLATE/bug_report.md) and include:
- Clear description of the issue
- Steps to reproduce
- Expected vs. actual behavior
- Screenshots or logs if applicable
- Environment details (OS, versions, hardware)

### Suggesting Features

Use the [feature request template](.github/ISSUE_TEMPLATE/feature_request.md) and include:
- Clear use case and problem statement
- Proposed solution
- Alternative approaches considered
- Impact on existing functionality

### Submitting Pull Requests

1. Fork the repository and create a branch from `main`
2. Make your changes following our conventions (see below)
3. Test your changes thoroughly
4. Update documentation as needed
5. Submit a PR using the [PR template](.github/PULL_REQUEST_TEMPLATE.md)

## 🏗️ Development Setup

### Mobile App (Flutter)
```bash
cd app
flutter pub get
flutter run
```

### Web Dashboard (React)
```bash
cd web
npm install
npm run dev
```

### Backend (Python)
```bash
cd backend
pip install -r requirements.txt
python -m pytest  # Run tests
```

### Firmware (Arduino/PlatformIO)
```bash
cd firmware
# Using PlatformIO
pio run
pio run --target upload
```

## 📝 Coding Conventions

### General
- Write clear, self-documenting code
- Add comments for complex logic
- Keep functions small and focused
- Follow DRY (Don't Repeat Yourself)

### JavaScript/React (Web)
- Use ES6+ features
- Follow Airbnb style guide
- Use functional components with hooks
- PropTypes or TypeScript for type checking
- Run `npm run lint` before committing

### Dart/Flutter (Mobile)
- Follow official Dart style guide
- Use meaningful widget names
- Extract reusable widgets
- Run `flutter analyze` before committing

### Python (Backend)
- Follow PEP 8 style guide
- Use type hints where applicable
- Write docstrings for public functions
- Run `black` and `flake8` before committing

### C++ (Firmware)
- Use meaningful variable names
- Comment hardware-specific code
- Keep loop() function minimal
- Document pin configurations

## 🌿 Branch Naming Convention

Use descriptive branch names following this pattern:
- `feature/short-description` - New features
- `fix/short-description` - Bug fixes
- `docs/short-description` - Documentation updates
- `refactor/short-description` - Code refactoring
- `test/short-description` - Test additions/updates
- `chore/short-description` - Build, CI, or tooling changes

Examples:
- `feature/mqtt-reconnection`
- `fix/power-reading-overflow`
- `docs/update-api-endpoints`

## 💬 Commit Message Style

Follow the Conventional Commits specification:

```
<type>(<scope>): <subject>

<body>

<footer>
```

**Types**: `feat`, `fix`, `docs`, `style`, `refactor`, `test`, `chore`

**Examples**:
```
feat(app): add real-time power graph widget

Implements line chart using fl_chart package to display
power consumption over the last 24 hours.

Closes #42
```

```
fix(firmware): correct INA219 calibration value

The previous value caused readings to be off by 10%.
Updated to manufacturer recommended calibration.

Fixes #38
```

## ✅ Pull Request Checklist

Before submitting a PR, ensure:
- [ ] Code follows project conventions
- [ ] All tests pass locally
- [ ] New features include tests
- [ ] Documentation is updated
- [ ] Commit messages follow convention
- [ ] PR description explains changes clearly
- [ ] Related issues are linked

## 🧪 Testing Requirements

### Unit Tests
- Write tests for new functionality
- Maintain or improve code coverage
- Test edge cases and error conditions

### Integration Tests
- Test component interactions
- Verify API contracts
- Test hardware communication (where applicable)

### Manual Testing
- Test on target devices/platforms
- Verify UI/UX changes
- Test with real hardware when available

## 🔍 Code Review Process

1. PRs require at least one approval
2. Address review comments promptly
3. Keep discussions constructive
4. Update PR based on feedback
5. Maintainers will merge when ready

## 📞 Getting Help

- **Questions**: Open a GitHub Discussion
- **Bugs**: Create an issue with bug template
- **Chat**: (Discord/Slack link TBD)
- **Email**: (Project email TBD)

## 🎯 Good First Issues

Look for issues labeled `good-first-issue` or `help-wanted` to get started!

## 🙏 Recognition

Contributors will be recognized in:
- GitHub contributors page
- Release notes for significant contributions
- Project documentation (hall of fame coming soon)

Thank you for making Smart Plug AI better!
