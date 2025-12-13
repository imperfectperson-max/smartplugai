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
5. Run security scans if touching security-critical code
6. Submit a PR using the [PR template](.github/PULL_REQUEST_TEMPLATE.md)

### Security Contributions

Security is our top priority. If you're contributing to security-related code or documentation:

#### Reporting Security Vulnerabilities

**DO NOT** create public GitHub issues for security vulnerabilities.

Instead:
1. **Email** security@smartplugai.com with details
2. **Use PGP** encryption if possible (key available on request)
3. **Include**:
   - Description of the vulnerability
   - Steps to reproduce
   - Potential impact
   - Suggested fix (if any)
   
We follow **responsible disclosure** principles:
- You'll receive acknowledgment within 24 hours
- We'll provide a timeline for fixes (typically 7-14 days for critical issues)
- Credit will be given in release notes (with your permission)
- **Bug bounty program**: R5,000 - R50,000 for critical vulnerabilities (launching Phase 2)

#### Security Code Contributions

When contributing security-related code:
- [ ] Follow secure coding guidelines (OWASP IoT Top 10)
- [ ] Never commit secrets, keys, or credentials
- [ ] Use parameterized queries (prevent SQL injection)
- [ ] Validate all inputs (prevent XSS)
- [ ] Implement rate limiting for authentication endpoints
- [ ] Add comprehensive error handling (no stack traces in production)
- [ ] Write security-focused unit tests
- [ ] Document security assumptions and threat model
- [ ] Run static analysis tools (CodeQL, Bandit, ESLint)
- [ ] Test with security tools (OWASP ZAP, Burp Suite)

#### Security Documentation Contributions

Help improve our security documentation:
- **docs/SECURITY.md**: Security architecture, threat model, compliance
- **Appendix E**: Security specifications (TLS config, certificate management)
- **Appendix F**: Security audit results and remediation
- **Security checklists**: Deployment, configuration, incident response

Label your security PRs with: `security`, `security-enhancement`, or `vulnerability-fix`

#### Security Audit Results

Our security audit results (Appendix F) are shared transparently with the community. If you identify issues during review, please report them via security@smartplugai.com.

#### Cybersecurity Team & CSO Role

We're building a dedicated security team:
- **CSO (Chief Security Officer)**: TBD - Cybersecurity specialist role (Phase 1-2 hiring priority)
- **Security Engineers**: Penetration testing, vulnerability management
- **DevSecOps**: Secure CI/CD, automated scanning

Interested in joining? Contact: careers@smartplugai.com

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
- Maintain or improve code coverage (target: 85% for security-critical code)
- Test edge cases and error conditions
- **Security tests**: Test authentication, authorization, input validation, encryption

### Integration Tests
- Test component interactions
- Verify API contracts
- Test hardware communication (where applicable)
- **Security integration tests**: End-to-end encryption, signed commands, device attestation

### Security Testing
- **Static Analysis**: Run CodeQL, Bandit (Python), ESLint (JavaScript)
- **Dependency Scanning**: Check for known vulnerabilities in dependencies
- **Penetration Testing**: Test authentication bypass, injection attacks, privilege escalation
- **Fuzzing**: Test input validation with malformed data
- **Hardware Security**: Test tamper detection, secure boot, flash encryption

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
- **Security Issues**: Email security@smartplugai.com (DO NOT create public issues)
- **Chat**: (Discord/Slack link TBD)
- **Email**: (Project email TBD)

## 🎯 Good First Issues

Look for issues labeled `good-first-issue`, `help-wanted`, or `security` to get started!

**Security-focused good first issues** might include:
- Adding security headers to API responses
- Improving input validation
- Writing security documentation
- Creating security test cases
- Reviewing and updating dependencies

## 🙏 Recognition

Contributors will be recognized in:
- GitHub contributors page
- Release notes for significant contributions
- Security hall of fame (for responsible vulnerability disclosure)
- Project documentation (hall of fame coming soon)

Thank you for making Smart Plug AI better and more secure!
